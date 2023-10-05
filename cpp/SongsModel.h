#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QDateTime>

enum class Proficiency
{
    Low,
    Medium,
    High,
};

struct Song
{
    QString name;
    QDateTime lastPracticed;
    Proficiency proficiency;
};

class SongsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString instrument READ instrument WRITE setInstrument NOTIFY instrumentChanged FINAL)

public:

    enum Roles
    {
        Name,
        LastPracticed,
        ProficiencyRole,
    };

    explicit SongsModel(QObject *parent = nullptr);

    const QString &instrument() const { return m_instrument; }

    void setInstrument(const QString &instrument);
    Q_INVOKABLE void addSong(const QString &name, Proficiency proficiency);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_songs.size(); }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void instrumentChanged();

private:
    QString m_instrument;
    QList<Song> m_songs;
//    QList<QString> m_links;
};
