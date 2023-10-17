// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "FaviconProvider.h"
#include "Settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("Muziko"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.0.1"));
    QCoreApplication::setOrganizationName(QStringLiteral("LorenDB"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("lorendb.dev"));

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
    engine.addImageProvider(QStringLiteral("favicon"), new FaviconProvider);
    engine.loadFromModule(QStringLiteral("dev.lorendb.muziko"), QStringLiteral("Main"));

    return app.exec();
}
