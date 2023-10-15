// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Muziko");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCoreApplication::setOrganizationName("LorenDB");
    QCoreApplication::setOrganizationDomain("lorendb.dev");

    QGuiApplication app(argc, argv);

    // Prime the settings singleton
    Settings::instance();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("dev.lorendb.muziko", "Main");

    return app.exec();
}
