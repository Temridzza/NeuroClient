#ifndef CHATGPT_MESSAGEREPOSITORY_H
#define CHATGPT_MESSAGEREPOSITORY_H

#pragma once

#include <QString>
#include <QList>
#include <QVariantMap>

class MessageRepository {
public:
    QString addMessage(
        const QString& chatId,
        const QString& role,
        const QString& content,

        const QString& providerId,
        const QString& modelId,

        const QString& providerSnapshot,
        const QString& modelSnapshot,

        const QString& parentId = ""
        );

    QList<QVariantMap> getMessages(
        const QString& chatId
        );
};

#endif // CHATGPT_MESSAGEREPOSITORY_H