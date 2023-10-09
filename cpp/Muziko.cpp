// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include "Muziko.h"

#include <QSettings>

Muziko::Muziko(QObject *parent)
    : QAbstractListModel{parent}
{
    loadSongs();
    if (m_songsModels.size() > 0)
        m_filter = new SongsFilterModel{m_songsModels[m_currentModel]};
}

Muziko::~Muziko()
{
    saveSongs();
}

Muziko *Muziko::instance()
{
    if (!s_instance)
        s_instance = new Muziko;
    return s_instance;
}

Muziko *Muziko::create(QQmlEngine *engine, QJSEngine *)
{
    // The instance has to exist before it is used. We cannot replace it.
    Q_ASSERT(s_instance);

    // The engine has to have the same thread affinity as the singleton.
    Q_ASSERT(engine->thread() == s_instance->thread());

    // There can only be one engine accessing the singleton.
    static QJSEngine *s_engine = engine;
    if (s_engine)
        Q_ASSERT(engine == s_engine);
    else
        s_engine = engine;

    // Explicitly specify C++ ownership so that the engine doesn't delete
    // the instance.
    QJSEngine::setObjectOwnership(s_instance, QJSEngine::CppOwnership);
    return s_instance;
}

QHash<int, QByteArray> Muziko::roleNames() const
{
    return {{Roles::Name, "name"}};
}

QVariant Muziko::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_songsModels.size())
        return {};

    switch (role)
    {
    case Roles::Name:
        return m_songsModels[index.row()]->instrument();
    default:
        return {};
    }
}

SongsModel *Muziko::songs() const
{
    return m_songsModels.value(m_currentModel, nullptr);
}

void Muziko::loadSongs()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("instruments"));
    const auto instruments = settings.childGroups();
    for (const auto &instrument : instruments)
    {
        auto model = new SongsModel{this};
        model->setInstrument(instrument);
        settings.beginGroup(instrument);
        const auto songs = settings.childGroups();
        for (const auto &song : songs)
        {
            auto s = new Song{this};
            s->setName(song);
            settings.beginGroup(song);
            s->setLastPracticed(settings.value(QStringLiteral("last_practiced")).toDateTime());
            s->setProficiency(settings.value(QStringLiteral("proficiency"), Song::Proficiency::MediumProficiency)
                                  .value<Song::Proficiency>());
            settings.endGroup(); // song
            model->addSong(s);
        }
        m_songsModels.push_back(model);

        // these must be connected *after* all the songs are added, or else saveSongs() will delete all the saved songs
        // before they are all loaded
        connect(model, &SongsModel::rowsInserted, this, &Muziko::saveSongs);
        connect(model, &SongsModel::rowsRemoved, this, &Muziko::saveSongs);
        connect(model, &SongsModel::dataChanged, this, &Muziko::saveSongs);
        settings.endGroup(); // instrument
    }
    settings.endGroup(); // instruments

    m_currentModel = -1;
    setCurrentInstrument(settings.value("last_used_instrument").toString());
    if (m_currentModel == -1 && m_songsModels.size() > 0)
        setCurrentInstrument(m_songsModels[0]->instrument());
}

void Muziko::saveSongs() const
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("instruments"));
    settings.remove("");
    for (const auto model : m_songsModels)
    {
        settings.beginGroup(model->instrument());
        // make sure the instrument group is written even if there are no songs
        settings.setValue(QStringLiteral("placeholder"), true);
        for (const auto &song : model->songs())
        {
            settings.beginGroup(song->name());
            settings.setValue(QStringLiteral("last_practiced"), song->lastPracticed());
            settings.setValue(QStringLiteral("proficiency"), song->proficiency());
            settings.endGroup(); // song.name
        }
        settings.endGroup(); // model->instrument()
    }
    settings.endGroup(); // instruments
}

void Muziko::addInstrument(const QString &name)
{
    auto model = new SongsModel{this};
    model->setInstrument(name);
    connect(model, &SongsModel::rowsInserted, this, &Muziko::saveSongs);
    connect(model, &SongsModel::rowsRemoved, this, &Muziko::saveSongs);
    connect(model, &SongsModel::dataChanged, this, &Muziko::saveSongs);
    m_currentModel = m_songsModels.size();
    beginInsertRows({}, m_currentModel, m_currentModel);
    m_songsModels.push_back(model);
    endInsertRows();
    emit songsChanged();
}

void Muziko::setCurrentInstrument(const QString &name)
{
    for (int i = 0; i < m_songsModels.size(); ++i)
    {
        if (m_songsModels[i]->instrument() == name)
        {
            m_currentModel = i;
            if (m_filter)
                m_filter->deleteLater();
            m_filter = new SongsFilterModel{m_songsModels[i]};
            emit songsChanged();
            QSettings settings;
            settings.setValue("last_used_instrument", m_songsModels[i]->instrument());
            break;
        }
    }
}

bool Muziko::alreadyHasInstrument(const QString &name) const
{
    return std::find_if(m_songsModels.begin(), m_songsModels.end(), [&name](SongsModel *s) {
               return s->instrument().toLower() == name.toLower();
           }) != m_songsModels.end();
}
