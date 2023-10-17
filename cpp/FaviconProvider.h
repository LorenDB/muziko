#pragma once

#include <QQuickAsyncImageProvider>

class FaviconProvider : public QQuickAsyncImageProvider
{
    Q_OBJECT

public:
    FaviconProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;
};
