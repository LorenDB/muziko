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

    m_dailySetSize = settings.value(SettingsKeys::DAILY_SET_SIZE, 5).toInt();
    m_mappingsCalculatedForSetSize = settings.value(SettingsKeys::MAPPINGS_CALCULATED_FOR_SET_SIZE, m_dailySetSize).toInt();
    m_useAmoledTheme = settings.value(SettingsKeys::USE_AMOLED_THEME, false).toBool();

    // in case we are loading new settings from file, we need to emit the signals to force updates to occur
    emit dailySetSizeChanged();
    emit mappingsCalculatedForSetSizeChanged();
    emit useAmoledThemeChanged();
}

void Settings::save() const
{
    QSettings settings;

    settings.setValue(SettingsKeys::SETTINGS_VERSION, MUZIKO_VERSION_STR);
    settings.setValue(SettingsKeys::DAILY_SET_SIZE, m_dailySetSize);
    settings.setValue(SettingsKeys::MAPPINGS_CALCULATED_FOR_SET_SIZE, m_mappingsCalculatedForSetSize);
    settings.setValue(SettingsKeys::USE_AMOLED_THEME, m_useAmoledTheme);
}

void Settings::setDailySetSize(int dailySetSize)
{
    if (m_dailySetSize == dailySetSize)
        return;
    m_dailySetSize = dailySetSize;
    emit dailySetSizeChanged();
}

void Settings::setMappingsCalculatedForSetSize(int setSize)
{
    if (m_mappingsCalculatedForSetSize == setSize)
        return;
    m_mappingsCalculatedForSetSize = setSize;
    emit mappingsCalculatedForSetSizeChanged();
}

void Settings::setUseAmoledTheme(bool useAmoledTheme)
{
    if (m_useAmoledTheme == useAmoledTheme)
        return;
    m_useAmoledTheme = useAmoledTheme;
    emit useAmoledThemeChanged();
}

void Settings::backup(const QString &path) const
{
    const QString realPath = QUrl{path}.toLocalFile();
    save();
    Muziko::instance()->saveSongs();

    if (!QFileInfo::exists(realPath))
    {
        QDir().mkpath(QFileInfo(realPath).dir().path());
        QFile f{realPath};
        f.open(QIODevice::WriteOnly);
        f.close();
    }

    QSettings settings;
    QSettings backupSettings{realPath, QSettings::IniFormat};
    const auto keys = settings.allKeys();
    for (const auto &key : keys)
        backupSettings.setValue(key, settings.value(key));
}

void Settings::restore(const QString &path)
{
    const QString realPath = QUrl{path}.toLocalFile();

    QSettings settings;
    QSettings backupSettings{realPath, QSettings::IniFormat};
    settings.clear();
    const auto keys = backupSettings.allKeys();
    for (const auto &key : keys)
        settings.setValue(key, backupSettings.value(key));

    settings.sync();
    load();
    Muziko::instance()->loadSongs();
}
