// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include "SongsModel.h"

#include "Settings.h"

Song::Song(QObject *parent)
    : QObject{parent}
{}

QString Song::lastPracticedString() const
{
    if (m_lastPracticed.isNull())
        return tr("Never");
    else if (m_lastPracticed.date().year() != QDate::currentDate().year())
        return m_lastPracticed.toString("MMMM d yyyy");
    else
        return m_lastPracticed.toString("MMMM d");
}

QString Song::proficiencyString() const
{
    switch (m_proficiency)
    {
    case Proficiency::LowProficiency:
        return tr("Low");
    case Proficiency::MediumProficiency:
        return tr("Medium");
    case Proficiency::HighProficiency:
        return tr("High");
    default:
        return {};
    }
}

void Song::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

void Song::setLastPracticed(const QDateTime &newLastPracticed)
{
    if (m_lastPracticed == newLastPracticed)
        return;
    setPracticeBeforeLast(m_lastPracticed);
    m_lastPracticed = newLastPracticed;
    emit lastPracticedChanged();
}

void Song::setPracticeBeforeLast(const QDateTime &newDateTime)
{
    if (m_practiceBeforeLast == newDateTime)
        return;
    m_practiceBeforeLast = newDateTime;
    emit practiceBeforeLastChanged();
}

void Song::setDailySet(const QDate &day)
{
    if (m_dailySet == day)
        return;
    m_dailySet = day;
    emit partOfTodaysSetChanged();
}

void Song::setPracticedToday(bool state)
{
    if (state && m_lastPracticed.date() != QDate::currentDate())
        setLastPracticed(QDateTime::currentDateTime());
    else if (!state && m_lastPracticed.date() == QDate::currentDate())
    {
        // Manually set m_lastPracticed to avoid changing m_practiceBeforeLast
        m_lastPracticed = m_practiceBeforeLast;
        emit lastPracticedChanged();
    }
}

void Song::addLink(const QString &link)
{
    if (m_links.contains(link))
        return;
    QUrl u{link};
    if (!u.isValid())
        return;
    if (u.scheme().isEmpty())
        u.setScheme(QStringLiteral("https"));
    if (u.isEmpty())
        return;

    m_links.push_back(u.toString());
    emit linksChanged();
}

void Song::removeLink(const QString &link)
{
    if (!m_links.contains(link))
        return;
    m_links.removeAll(link);
    emit linksChanged();
}

void Song::setProficiency(Proficiency newProficiency)
{
    if (m_proficiency == newProficiency)
        return;
    m_proficiency = newProficiency;
    emit proficiencyChanged();
}

void Song::setLinks(const QStringList &links)
{
    if (m_links == links)
        return;
    m_links = links;
    emit linksChanged();
}

SongsModel::SongsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SongsModel::setInstrument(const QString &instrument)
{
    if (instrument == m_instrument)
        return;
    m_instrument = instrument;
    emit instrumentChanged();
}

void SongsModel::addSong(const QString &name, Song::Proficiency proficiency)
{
    auto s = new Song{this};
    s->setName(name);
    s->setProficiency(proficiency);
    addSong(s);
}

void SongsModel::addSong(Song *song)
{
    beginInsertRows({}, m_songs.size(), m_songs.size());
    m_songs.push_back(song);
    endInsertRows();

    auto updateSong = [this, song](Roles role) {
        for (int i = 0; i < m_songs.size(); ++i)
        {
            if (m_songs[i] == song)
            {
                emit dataChanged(index(i), index(i), {role});
                break;
            }
        }
    };
    connect(song, &Song::nameChanged, this, [updateSong] { updateSong(Roles::Name); });
    connect(song, &Song::lastPracticedChanged, this, [updateSong] { updateSong(Roles::LastPracticed); });
    connect(song, &Song::proficiencyChanged, this, [updateSong] { updateSong(Roles::ProficiencyValue); });
    connect(song, &Song::linksChanged, this, [updateSong] { updateSong(Roles::Links); });
}

void SongsModel::removeSong(const QString &name)
{
    for (int i = 0; i < m_songs.size(); ++i)
    {
        if (m_songs[i]->name() == name)
        {
            beginRemoveRows({}, i, i);
            m_songs[i]->deleteLater();
            m_songs.remove(i);
            endRemoveRows();
            break;
        }
    }
}

QHash<int, QByteArray> SongsModel::roleNames() const
{
    return {{Roles::Name, "name"},
            {Roles::LastPracticed, "lastPracticedString"},
            {Roles::ProficiencyValue, "proficiency"},
            {Roles::Links, "links"},
            {Roles::SongObject, "song"}};
}

QVariant SongsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_songs.size())
        return {};

    switch (role)
    {
    case Roles::Name:
        return m_songs[index.row()]->name();
    case Roles::LastPracticed:
        return m_songs[index.row()]->lastPracticedString();
    case Roles::ProficiencyValue:
        return m_songs[index.row()]->proficiency();
    case Roles::Links:
        return m_songs[index.row()]->links();
    case Roles::SongObject:
        return QVariant::fromValue(m_songs[index.row()]);
    default:
        return {};
    }
}

SongsFilterModel::SongsFilterModel(SongsModel *parent)
    : QAbstractProxyModel{parent},
      m_model{parent}
{
    setSourceModel(parent);
    rebuildMappings();

    m_beginningOfDayUpdate.setInterval(
        QDateTime::currentDateTime().msecsTo(QDateTime{QDate::currentDate().addDays(1), QTime{}}) + 5);
    m_beginningOfDayUpdate.setSingleShot(true);
    m_beginningOfDayUpdate.callOnTimeout([this] {
       rebuildMappings();
       m_beginningOfDayUpdate.setInterval(
           QDateTime::currentDateTime().msecsTo(QDateTime{QDate::currentDate().addDays(1), QTime{}}) + 5);
       m_beginningOfDayUpdate.start();
    });
    m_beginningOfDayUpdate.start();

    connect(
        parent, &SongsModel::rowsInserted, this, [this](const QModelIndex &, int first, int last) { rebuildMappings(); });
    connect(parent, &SongsModel::rowsRemoved, this, [this](const QModelIndex &, int first, int last) { rebuildMappings(); });
    connect(parent,
            &SongsModel::dataChanged,
            this,
            [this](const QModelIndex &left, const QModelIndex &right, const QList<int> &roles) {
                if (roles.size() == 1)
                {
                    auto role = roles[0];
                    switch (role)
                    {
                    case SongsModel::Name:
                        for (int i = left.row(); i <= right.row(); ++i)
                        {
                            if (m_mappings.contains(i))
                            {
                                auto idx = index(m_mappings.indexOf(i));
                                emit dataChanged(idx, idx, roles);
                            }
                        }
                        break;
                    case SongsModel::LastPracticed:
                        for (int sourceIdx = left.row(); sourceIdx <= right.row(); ++sourceIdx)
                        {
                            if (m_mappings.contains(sourceIdx))
                            {
                                auto proxyIdx = m_mappings.indexOf(sourceIdx);
                                Song *song =
                                    m_model->data(m_model->index(sourceIdx), SongsModel::SongObject).value<Song *>();
                                int targetLoc = 0;
                                for (; targetLoc < m_mappings.size(); ++targetLoc)
                                {
                                    if (targetLoc == proxyIdx)
                                        continue;
                                    auto cmp = m_model->data(m_model->index(m_mappings[targetLoc]), SongsModel::SongObject)
                                                   .value<Song *>();

                                    bool greaterThan = false;
                                    if (!song->lastPracticed().isNull() && cmp->lastPracticed().isNull())
                                        greaterThan = true;
                                    else if (song->lastPracticed().date() > cmp->lastPracticed().date())
                                        greaterThan = true;
                                    else if (song->lastPracticed().date() == cmp->lastPracticed().date())
                                    {
                                        if (song->proficiency() > cmp->proficiency())
                                            greaterThan = true;
                                        else if (song->proficiency() == cmp->proficiency())
                                            if (song->name() > cmp->name())
                                                greaterThan = true;
                                    }

                                    if (!greaterThan)
                                        break;
                                }

                                if (targetLoc == m_mappings.size())
                                    --targetLoc;
                                if (proxyIdx == targetLoc)
                                    continue;

                                // So... QAbstractListModel::beginMoveRows() has some *wacky* behavior if you are moving a
                                // row to a different location in the same model. Specifically, moving anywhere other than 0
                                // requires inserting at the index *after* where you want to be, but only if you are moving
                                // down the list. I don't get it. Why can't you just use a normal move like QList has???
                                beginMoveRows({}, proxyIdx, proxyIdx, {}, targetLoc > proxyIdx ? targetLoc + 1 : targetLoc);
                                m_mappings.move(proxyIdx, targetLoc);
                                endMoveRows();
                            }
                        }

                        if (std::all_of(m_mappings.begin(), m_mappings.end(), [this](const int idx) {
                                return m_model->data(m_model->index(idx), SongsModel::SongObject)
                                           .value<Song *>()
                                           ->lastPracticed()
                                           .date() == QDate::currentDate();
                            }))
                            emit dailySetDone();
                        break;
                    case SongsModel::ProficiencyValue:
                    case SongsModel::SongObject:
                    default:
                        rebuildMappings();
                        break;
                    }
                }
                else
                    rebuildMappings();
            });
    connect(Settings::instance(), &Settings::dailySetSizeChanged, this, &SongsFilterModel::rebuildMappings);
}

QModelIndex SongsFilterModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    for (int i = 0; i < m_mappings.size(); ++i)
        if (m_mappings[i] == sourceIndex.row())
            return index(i, sourceIndex.column(), sourceIndex.parent());
    return {};
}

QModelIndex SongsFilterModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (proxyIndex.row() >= 0 && proxyIndex.row() < m_mappings.size())
        return index(m_mappings[proxyIndex.row()], proxyIndex.column(), proxyIndex.parent());
    return {};
}

QModelIndex SongsFilterModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

QModelIndex SongsFilterModel::parent(const QModelIndex &) const
{
    return {};
}

int SongsFilterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_mappings.size();
}

int SongsFilterModel::columnCount(const QModelIndex &) const
{
    return 1;
}

void SongsFilterModel::rebuildMappings()
{
    beginResetModel();

    m_mappings.clear();
    decltype(m_mappings) alreadyPracticedMappings;
    const auto dailySetSize = Settings::instance()->dailySetSize();

    // if the user practiced this song earlier today as part of a set, we want to temporarily include that in the list so
    // we don't spam new songs on the list infinitely
    auto getPracticeDate = [](Song *song) {
        return (song->dailySet() == QDate::currentDate() && song->lastPracticed().date() == QDate::currentDate() ?
                    song->practiceBeforeLast() :
                    song->lastPracticed());
    };

    auto sortedSongs = m_model->songs();
    std::sort(sortedSongs.begin(), sortedSongs.end(), [&getPracticeDate](Song *lhs, Song *rhs) {
        return getPracticeDate(lhs) < getPracticeDate(rhs);
    });

    auto low = sortedSongs;
    low.removeIf([&getPracticeDate](Song *song) {
        auto date = getPracticeDate(song);
        return song->proficiency() != Song::LowProficiency ||
               (date.isValid() && date.daysTo(QDateTime::currentDateTime()) <= 0);
    });

    int minDaysSinceMedPractice;
    if (low.size() >= dailySetSize * 4 / 5 - 1)
        minDaysSinceMedPractice = 4;
    else
        minDaysSinceMedPractice = 3;
    auto med = sortedSongs;
    med.removeIf([&minDaysSinceMedPractice, &getPracticeDate](Song *song) {
        auto date = getPracticeDate(song);
        return song->proficiency() != Song::MediumProficiency ||
               (date.isValid() && date.daysTo(QDateTime::currentDateTime()) < minDaysSinceMedPractice);
    });

    auto high = sortedSongs;
    high.removeIf([](Song *song) { return song->proficiency() != Song::HighProficiency; });

    bool isDailySetSizeOverflowed = low.size() + med.size() >= dailySetSize;

    if (high.size() > 0)
    {
        int numToAppend = std::min(
            isDailySetSizeOverflowed ? std::min(dailySetSize / 5, 2) : dailySetSize - low.size() - med.size(), high.size());
        for (int i = 0; i < numToAppend; ++i)
        {
            if (!high[i]->practicedToday())
                m_mappings.append(m_model->songs().indexOf(high[i]));
            else
                alreadyPracticedMappings.append(m_model->songs().indexOf(high[i]));
        }
    }

    if (med.size() > 0)
    {
        int numToAppend = isDailySetSizeOverflowed ? dailySetSize / 5 : med.size();
        for (int i = numToAppend - 1; i >= 0; --i)
        {
            if (!med[i]->practicedToday())
                m_mappings.prepend(m_model->songs().indexOf(med[i]));
            else
                alreadyPracticedMappings.prepend(m_model->songs().indexOf(med[i]));
        }
    }

    if (low.size() > 0)
        for (int i = std::min(low.size(), dailySetSize - m_mappings.size()) - 1; i >= 0; --i)
        {
            if (!low[i]->practicedToday())
                m_mappings.prepend(m_model->songs().indexOf(low[i]));
            else
                alreadyPracticedMappings.prepend(m_model->songs().indexOf(low[i]));
        }

    m_mappings.append(alreadyPracticedMappings);

    for (const auto i : m_mappings)
        m_model->songs()[i]->setDailySet(QDate::currentDate());

    endResetModel();
}
