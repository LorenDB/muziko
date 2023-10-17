#include "FaviconProvider.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QFile>

class FaviconResponse : public QQuickImageResponse
{
public:
    FaviconResponse(const QUrl &url, const QSize &requestedSize)
    {
        if (!url.isValid())
        {
            emit finished();
            return;
        }

        auto *nam = new QNetworkAccessManager{this};
        QNetworkRequest req{url};
        auto rep = nam->get(req);
        connect(rep, &QNetworkReply::finished, this, [this, rep, requestedSize, url] {
            if (rep->error() == QNetworkReply::NoError)
            {
                auto data = rep->readAll();
                auto cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + '/' + url.host() + ".ico";
                QFile cache{cachePath};
                if (cache.open(QIODevice::WriteOnly))
                {
                    cache.write(data);
                    cache.close();
                }
                m_image = QPixmap(cachePath).toImage();
                if (!requestedSize.isEmpty())
                    m_image = m_image.scaled(requestedSize, Qt::KeepAspectRatio);
            }
            emit finished();
        });
    }

    QQuickTextureFactory *textureFactory() const override
    {
        return QQuickTextureFactory::textureFactoryForImage(m_image);
    }

private:
    QImage m_image;
};

FaviconProvider::FaviconProvider() {}

QQuickImageResponse *FaviconProvider::requestImageResponse(const QString &id, const QSize &requestedSize)
{
    QUrl url{id};
    url.setPath("/favicon.ico");
    return new FaviconResponse{url, requestedSize};
}
