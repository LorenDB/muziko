// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Muziko.h"

#include <QSettings>

#include "Settings.h"

#ifdef Q_OS_ANDROID
    #include <QCoreApplication>
    #include <QJniEnvironment>
    #include <QJniObject>
#endif

Muziko::Muziko(QObject *parent)
    : QAbstractListModel{parent}
{
    if (!s_instance)
        s_instance = this;

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
        new Muziko;
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
    beginResetModel();
    for (const auto model : m_songsModels)
        model->deleteLater();
    m_songsModels.clear();

    QSettings settings;

    QString settingsVersion = settings.value(SettingsKeys::SETTINGS_VERSION).toString();

    settings.beginGroup(SettingsKeys::INSTRUMENTS);
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

            // TODO: remove this migration at some point. It was added for 0.1.1, so probably it can be removed by 0.3.0
            // safely (especially given the small user base, which means that there is a low chance of users missing an
            // update). However, it's always better safe than sorry, so maybe it should stick around for a little longer than
            // that.
            if (settingsVersion == QStringLiteral("0.1.0") && !settings.contains(SettingsKeys::PRACTICES))
                s->setLastPracticed(settings.value(SettingsKeys::LAST_PRACTICED).toDateTime());
            else
            {
                auto practiceVariant = settings.value(SettingsKeys::PRACTICES).toList();
                QList<QDateTime> practices;
                for (const auto &p : practiceVariant)
                    practices.append(p.toDateTime());
                s->setPractices(practices);
            }

            s->setProficiency(
                settings.value(SettingsKeys::PROFICIENCY, Song::Proficiency::MediumProficiency).value<Song::Proficiency>());
            s->setDailySet(settings.value(SettingsKeys::DAILY_SET).toDate());
            s->setLinks(settings.value(SettingsKeys::LINKS).toStringList());
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
    setCurrentInstrument(settings.value(SettingsKeys::LAST_USED_INSTRUMENT).toString());
    if (m_currentModel == -1 && m_songsModels.size() > 0)
        setCurrentInstrument(m_songsModels[0]->instrument());

    endResetModel();
}

void Muziko::saveSongs() const
{
    QSettings settings;
    settings.beginGroup(SettingsKeys::INSTRUMENTS);
    settings.remove("");
    for (const auto model : m_songsModels)
    {
        settings.beginGroup(model->instrument());
        // make sure the instrument group is written even if there are no songs
        settings.setValue(QStringLiteral("placeholder"), true);
        for (const auto &song : model->songs())
        {
            settings.beginGroup(song->name());

            QVariantList practices;
            for (const auto &practice : song->practices())
                practices.push_back(practice);

            settings.setValue(SettingsKeys::PRACTICES, practices);
            settings.setValue(SettingsKeys::PROFICIENCY, song->proficiency());
            settings.setValue(SettingsKeys::DAILY_SET, song->dailySet());
            settings.setValue(SettingsKeys::LINKS, song->links());
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
            settings.setValue(SettingsKeys::LAST_USED_INSTRUMENT, m_songsModels[i]->instrument());
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

bool Muziko::isValidUrl(const QString &url)
{
    QUrl u{url};
    if (!u.isValid())
        return false;
    auto validated = u.toString();
    if (validated.isEmpty())
        return false;
    return true;
}

void Muziko::androidSetBlockSleep(bool block)
{
#ifdef Q_OS_ANDROID
    // This implementation copied from https://stackoverflow.com/a/38846485/12533859
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([block] {
        QJniObject context = QNativeInterface::QAndroidApplication::context();
        if (context.isValid())
        {
            QJniObject window = context.callObjectMethod("getWindow", "()Landroid/view/Window;");
            if (window.isValid())
            {
                const int FLAG_KEEP_SCREEN_ON = 128;
                if (block)
                    window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                else
                    window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
            }
        }
        QJniEnvironment env;
        if (env->ExceptionCheck())
            env->ExceptionClear();
    });
#else
    Q_UNUSED(block)
#endif
}
