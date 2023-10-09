// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include "SongsModel.h"

Song::Song(QObject *parent)
    : QObject{parent}
{}

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
    m_lastPracticed = newLastPracticed;
    emit lastPracticedChanged();
}

void Song::markAsPracticedToday()
{
    setLastPracticed(QDateTime::currentDateTime());
}

void Song::setProficiency(Proficiency newProficiency)
{
    if (m_proficiency == newProficiency)
        return;
    m_proficiency = newProficiency;
    emit proficiencyChanged();
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

    auto updateSong = [this, song] {
        for (int i = 0; i < m_songs.size(); ++i)
        {
            if (m_songs[i] == song)
            {
                emit dataChanged(index(i), index(i));
                break;
            }
        }
    };
    connect(song, &Song::nameChanged, this, updateSong);
    connect(song, &Song::lastPracticedChanged, this, updateSong);
    connect(song, &Song::proficiencyChanged, this, updateSong);
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
            {Roles::LastPracticed, "lastPracticed"},
            {Roles::ProficiencyValue, "proficiency"},
            {Roles::ProficiencyString, "proficiencyString"},
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
        return m_songs[index.row()]->lastPracticed();
    case Roles::ProficiencyValue:
        return m_songs[index.row()]->proficiency();
    case Roles::ProficiencyString:
    case Roles::SongObject:
        return QVariant::fromValue(m_songs[index.row()]);
    default:
        return {};
    }
}

SongsFilterModel::SongsFilterModel(SongsModel *parent)
    : QSortFilterProxyModel{parent},
      m_model{parent}
{
    setSourceModel(parent);
    sort(0);
}

bool SongsFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const auto &leftSong = m_model->songs()[left.row()];
    const auto &rightSong = m_model->songs()[right.row()];

    if (leftSong->proficiency() == Song::LowProficiency && rightSong->proficiency() != Song::LowProficiency)
        return true;

    return leftSong->lastPracticed() < rightSong->lastPracticed() || leftSong->proficiency() < rightSong->proficiency() ||
           leftSong->name() < rightSong->name();
}

bool SongsFilterModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    const auto &lp = m_model->songs()[row]->lastPracticed();
    return lp.date() < QDate::currentDate() || !lp.isValid();
}
