// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

namespace SettingsKeys
{
    constexpr auto DAILY_SET_SIZE = "daily_set_size";
    constexpr auto MAPPINGS_CALCULATED_FOR_SET_SIZE = "mappings_calculated_for_set_size";
    constexpr auto SETTINGS_VERSION = "settings_version";
    constexpr auto INSTRUMENTS = "instruments";
    constexpr auto LAST_PRACTICED = "last_practiced";
    constexpr auto PRACTICES = "practices";
    constexpr auto PROFICIENCY = "proficiency";
    constexpr auto DAILY_SET = "daily_set";
    constexpr auto LINKS = "links";
    constexpr auto LAST_USED_INSTRUMENT = "last_used_instrument";
} // namespace SettingsKeys

class Settings : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(MuzikoSettings)
    QML_SINGLETON

    Q_PROPERTY(int dailySetSize READ dailySetSize WRITE setDailySetSize NOTIFY dailySetSizeChanged FINAL)

public:
    ~Settings();

    static Settings *instance();
    static Settings *create(QQmlEngine *engine, QJSEngine *);

    void load();
    void save() const;

    int dailySetSize() const { return m_dailySetSize; }
    int mappingsCalculatedForSetSize() const { return m_mappingsCalculatedForSetSize; }

    void setDailySetSize(int dailySetSize);
    void setMappingsCalculatedForSetSize(int setSize);

    Q_INVOKABLE void backup() const;
    Q_INVOKABLE void restore();

signals:
    void dailySetSizeChanged();
    void mappingsCalculatedForSetSizeChanged();

private:
    explicit Settings(QObject *parent = nullptr);
    static inline Settings *s_instance;

    int m_dailySetSize;
    int m_mappingsCalculatedForSetSize;
};
