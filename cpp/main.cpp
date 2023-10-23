// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "FaviconProvider.h"
#include "Settings.h"
#include "cpp/MuzikoVersion.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("Muziko"));
    QCoreApplication::setApplicationVersion(QStringLiteral(MUZIKO_VERSION_STR));
    QCoreApplication::setOrganizationName(QStringLiteral("LorenDB"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("lorendb.dev"));

    QGuiApplication app(argc, argv);

    app.connect(&app, &QGuiApplication::aboutToQuit, Settings::instance(), &Settings::save);

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
