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
    QDateTime practiceBeforeLast() const { return m_practiceBeforeLast; }
    QDate partOfTodaysSet() const { return m_partOfTodaysSet; }
    Proficiency proficiency() const { return m_proficiency; }

    QString proficiencyString() const;

    void setName(const QString &newName);
    void setLastPracticed(const QDateTime &newLastPracticed);
    void setPracticeBeforeLast(const QDateTime &newDateTime);
    void setPartOfTodaysSet(const QDate &day);
    void setProficiency(Proficiency newProficiency);

    Q_INVOKABLE void markAsPracticedToday();

signals:
    void nameChanged();
    void lastPracticedChanged();
    void practiceBeforeLastChanged();
    void partOfTodaysSetChanged();
    void proficiencyChanged();

private:
    QString m_name;
    QDateTime m_lastPracticed;
    QDateTime m_practiceBeforeLast;
    QDate m_partOfTodaysSet;
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

class SongsFilterModel : public QAbstractProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit SongsFilterModel(SongsModel *parent = nullptr);

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex & = {}) const override;
    QModelIndex parent(const QModelIndex &) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex & = {}) const override;

    void rebuildMappings();

private:
    // The keys of this list map directly to the proxy rows. The value for each key is the source row.
    QList<int> m_mappings;
    int m_dailySetSize = 5;

    SongsModel *m_model;
};
