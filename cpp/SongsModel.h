// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QTimer>

class Song : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString lastPracticedString READ lastPracticedString NOTIFY lastPracticedChanged FINAL)
    Q_PROPERTY(bool practicedToday READ practicedToday WRITE setPracticedToday NOTIFY lastPracticedChanged FINAL)
    Q_PROPERTY(Proficiency proficiency READ proficiency WRITE setProficiency NOTIFY proficiencyChanged FINAL)
    Q_PROPERTY(QString proficiencyString READ proficiencyString NOTIFY proficiencyChanged FINAL)
    Q_PROPERTY(QStringList links READ links NOTIFY linksChanged FINAL)

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
    QString lastPracticedString() const;
    QDateTime practiceBeforeLast() const { return m_practiceBeforeLast; }
    QDate dailySet() const { return m_dailySet; }
    bool practicedToday() const { return m_lastPracticed.date() == QDate::currentDate(); }
    Proficiency proficiency() const { return m_proficiency; }
    QStringList links() const { return m_links; }

    QString proficiencyString() const;

    void setName(const QString &newName);
    void setLastPracticed(const QDateTime &newLastPracticed);
    void setPracticeBeforeLast(const QDateTime &newDateTime);
    void setDailySet(const QDate &day);
    void setProficiency(Proficiency newProficiency);
    void setLinks(const QStringList &links);

    void setPracticedToday(bool state);
    Q_INVOKABLE void addLink(const QString &link);
    Q_INVOKABLE void removeLink(const QString &link);

signals:
    void nameChanged();
    void lastPracticedChanged();
    void practiceBeforeLastChanged();
    void partOfTodaysSetChanged();
    void proficiencyChanged();
    void linksChanged();

private:
    QString m_name;
    QDateTime m_lastPracticed;
    QDateTime m_practiceBeforeLast;
    QDate m_dailySet;
    Proficiency m_proficiency;
    QStringList m_links;
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
        Links,
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

signals:
    void dailySetDone();

private:
    // The keys of this list map directly to the proxy rows. The value for each key is the source row.
    QList<int> m_mappings;
    SongsModel *m_model;
    QTimer m_beginningOfDayUpdate;
};
