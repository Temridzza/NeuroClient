#ifndef CHAT_H
#define CHAT_H

#pragma once
#include <QString>

struct Chat {
    QString id;
    QString title;
    QString prompt;

    QString providerId;
    QString modelId;

    QString rootMessageId;

    qint64 createdAt = 0;
    qint64 updatedAt = 0;
};

#endif // CHAT_H
