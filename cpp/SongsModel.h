// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QQmlEngine>
#include <QSortFilterProxyModel>

class Song : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QDateTime lastPracticed READ lastPracticed WRITE setLastPracticed NOTIFY lastPracticedChanged FINAL)
    Q_PROPERTY(Proficiency proficiency READ proficiency WRITE setProficiency NOTIFY proficiencyChanged FINAL)
    Q_PROPERTY(QString proficiencyString READ proficiencyString NOTIFY proficiencyChanged FINAL)

public:
    enum Proficiency
    {
        LowProficiency,
        MediumProficiency,
        HighProficiency,
    };
    Q_ENUM(Proficiency)

    explicit Song(QObject *parent = nullptr);

    QString name() const { return m_name; }
    QDateTime lastPracticed() const { return m_lastPracticed; }
    Proficiency proficiency() const { return m_proficiency; }

    QString proficiencyString() const;

    void setProficiency(Proficiency newProficiency);
    void setName(const QString &newName);
    void setLastPracticed(const QDateTime &newLastPracticed);

    Q_INVOKABLE void markAsPracticedToday();

signals:
    void nameChanged();
    void lastPracticedChanged();
    void proficiencyChanged();

private:
    QString m_name;
    QDateTime m_lastPracticed;
    Proficiency m_proficiency;
};
Q_DECLARE_METATYPE(Song)

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
        ProficiencyValue,
        ProficiencyString,
        SongObject,
    };

    explicit SongsModel(QObject *parent = nullptr);

    const QString &instrument() const { return m_instrument; }

    void setInstrument(const QString &instrument);
    Q_INVOKABLE void addSong(const QString &name, Song::Proficiency proficiency);
    Q_INVOKABLE void addSong(Song *song);
    Q_INVOKABLE void removeSong(const QString &name);

    const QList<Song *> &songs() const { return m_songs; }

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_songs.size(); }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void instrumentChanged();

private:
    QString m_instrument;
    QList<Song *> m_songs;
    //    QList<QString> m_links;
};

class SongsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit SongsFilterModel(SongsModel *parent = nullptr);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    bool filterAcceptsRow(int row, const QModelIndex &parent) const override;

private:
    SongsModel *m_model;
};
