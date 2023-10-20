// SPDX-FileCopyrightText: Loren Burkholder
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
//
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <QQuickAsyncImageProvider>

class FaviconProvider : public QQuickAsyncImageProvider
{
    Q_OBJECT

public:
    FaviconProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;
};
