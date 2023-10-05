// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "Muziko.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  // load the singleton instance
//  [[maybe_unused]] auto muziko = Muziko::instance();

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
      engine.loadFromModule("dev.lorendb.muziko", "Main");

  return app.exec();
}
