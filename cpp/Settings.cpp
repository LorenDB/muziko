// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include "Settings.h"

#include <QSettings>

#include "cpp/MuzikoVersion.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    load();
}

Settings::~Settings()
{
    save();
}

Settings *Settings::instance()
{
    if (!s_instance)
        s_instance = new Settings;
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

void Settings::save()
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
