// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#include <QDate>
#include <QDebug>
#include <QTimer>
#include <QtCore/private/qandroidextras_p.h>

#include "cpp/Muziko.h"

int main(int argc, char *argv[])
{
    QAndroidService service{argc, argv};

    qDebug() << "Starting the Muziko notifier service";

    QDateTime nextNotificationTime;

    bool needsImmediateNotification = QDateTime::currentDateTime().time().hour() < 3;
    if (needsImmediateNotification)
        nextNotificationTime = QDateTime{QDate::currentDate(), QTime{3, 0}};
    else
        nextNotificationTime = QDateTime{QDate::currentDate().addDays(1), QTime{3, 0}};

    QTimer notifyTimer;
    notifyTimer.setSingleShot(true);
    notifyTimer.setInterval(needsImmediateNotification ? 0 : QDateTime::currentDateTime().msecsTo(nextNotificationTime) + 3);
    notifyTimer.callOnTimeout([&nextNotificationTime, &notifyTimer] {
        // TODO: send notification

        nextNotificationTime = nextNotificationTime.addDays(1);
        notifyTimer.setInterval(QDateTime::currentDateTime().msecsTo(nextNotificationTime) + 3);
        notifyTimer.start();
    });
    notifyTimer.start();

    return service.exec();
}
