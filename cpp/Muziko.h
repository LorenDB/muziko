#pragma once

#include <QObject>
#include <QQmlEngine>

#include "SongsModel.h"

class Muziko : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SongsModel * songs READ songs NOTIFY songsChanged FINAL)

public:
    explicit Muziko(QObject *parent = nullptr);

    static Muziko *instance();
    static Muziko *create(QQmlEngine *engine, QJSEngine *);

    SongsModel *songs() const;

signals:
    void songsChanged();

private:
    static inline Muziko *s_instance;

    QList<SongsModel *> m_songsModels;
    size_t m_currentModel = 0;
};
