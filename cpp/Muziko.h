// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

#include "SongsModel.h"
#include "cpp/MuzikoVersion.h"

class Muziko : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(SongsModel *songs READ songs NOTIFY songsChanged FINAL)
    Q_PROPERTY(SongsFilterModel *songsFilter READ songsFilter NOTIFY songsChanged FINAL)

public:
    enum Roles
    {
        Name,
    };

    explicit Muziko(QObject *parent = nullptr);
    ~Muziko();

    static Muziko *instance();
    static Muziko *create(QQmlEngine *engine, QJSEngine *);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_songsModels.size(); }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    SongsModel *songs() const;
    SongsFilterModel *songsFilter() const { return m_filter; }

    void loadSongs();
    void saveSongs() const;

    Q_INVOKABLE void addInstrument(const QString &name);
    Q_INVOKABLE void setCurrentInstrument(const QString &name);

    Q_INVOKABLE bool alreadyHasInstrument(const QString &name) const;

    Q_INVOKABLE bool isValidUrl(const QString &url);

    Q_INVOKABLE QString muzikoVersion() const { return QStringLiteral(MUZIKO_VERSION_STR); }

signals:
    void songsChanged();

private:
    static inline Muziko *s_instance;

    QList<SongsModel *> m_songsModels;
    SongsFilterModel *m_filter = nullptr;
    size_t m_currentModel = 0;
};
