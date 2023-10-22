// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Settings.h"

#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

#include "Muziko.h"
#include "cpp/MuzikoVersion.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    if (!s_instance)
        s_instance = this;

    load();
}

Settings::~Settings()
{
    save();
}

Settings *Settings::instance()
{
    if (!s_instance)
        new Settings;
    return s_instance;
}

Settings *Settings::create(QQmlEngine *engine, QJSEngine *)
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

void Settings::load()
{
    QSettings settings;

    m_dailySetSize = settings.value(QStringLiteral("daily_set_size"), 5).toInt();
}

void Settings::save() const
{
    QSettings settings;

    settings.setValue(QStringLiteral("settings_version"), MUZIKO_VERSION_STR);
    settings.setValue(QStringLiteral("daily_set_size"), m_dailySetSize);
}

int Settings::dailySetSize() const
{
    return m_dailySetSize;
}

void Settings::setDailySetSize(int dailySetSize)
{
    if (m_dailySetSize == dailySetSize)
        return;
    m_dailySetSize = dailySetSize;
    emit dailySetSizeChanged();
}

void Settings::backup() const
{
    save();
    Muziko::instance()->saveSongs();

    const auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/muziko-backup.ini";

    if (!QFileInfo::exists(path))
    {
        QDir().mkpath(QFileInfo(path).dir().path());
        QFile f{path};
        f.open(QIODevice::WriteOnly);
        f.close();
    }

    QSettings settings;
    QSettings backupSettings{path, QSettings::IniFormat};
    const auto keys = settings.allKeys();
    for (const auto &key : keys)
        backupSettings.setValue(key, settings.value(key));
}

void Settings::restore()
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/muziko-backup.ini";

    QSettings settings;
    QSettings backupSettings{path, QSettings::IniFormat};
    settings.clear();
    const auto keys = backupSettings.allKeys();
    for (const auto &key : keys)
        settings.setValue(key, backupSettings.value(key));

    settings.sync();
    load();
    Muziko::instance()->loadSongs();
}
