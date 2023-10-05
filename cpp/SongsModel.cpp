#include "SongsModel.h"

SongsModel::SongsModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_songs.push_back(Song{.name = tr("Foggy Mountain Special"),
                           .lastPracticed = QDateTime::currentDateTime(),
                           .proficiency = Proficiency::Medium});
}

void SongsModel::setInstrument(const QString &instrument)
{
    if (instrument == m_instrument)
        return;
    m_instrument = instrument;
    emit instrumentChanged();
}

void SongsModel::addSong(const QString &name, Proficiency proficiency)
{
    beginInsertRows({}, m_songs.size(), m_songs.size());
    m_songs.push_back(Song{.name = name, .lastPracticed = QDateTime{}, .proficiency = proficiency});
    endInsertRows();
}

QHash<int, QByteArray> SongsModel::roleNames() const
{
    return {{Roles::Name, "name"}, {Roles::LastPracticed, "lastPracticed"}, {Roles::ProficiencyRole, "proficiency"}};
}

QVariant SongsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_songs.size())
        return {};

    switch (role)
    {
    case Roles::Name:
        return m_songs[index.row()].name;
    case Roles::LastPracticed:
        return m_songs[index.row()].lastPracticed;
    case Roles::ProficiencyRole:
        switch (m_songs[index.row()].proficiency)
        {
        case Proficiency::Low:
            return tr("Low");
        case Proficiency::Medium:
            return tr("Medium");
        case Proficiency::High:
            return tr("High");
        default:
            return {};
        }
    default:
        return {};
    }
}
