// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <QObject>
#include <QQmlEngine>

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

    int dailySetSize() const;
    void setDailySetSize(int dailySetSize);

    Q_INVOKABLE void backup() const;
    Q_INVOKABLE void restore();

signals:
    void dailySetSizeChanged();

private:
    explicit Settings(QObject *parent = nullptr);
    static inline Settings *s_instance;

    int m_dailySetSize;
};
