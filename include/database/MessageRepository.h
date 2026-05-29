#ifndef CHATGPT_MESSAGEREPOSITORY_H
#define CHATGPT_MESSAGEREPOSITORY_H

#pragma once

#include <QString>
#include <QList>
#include <QVariantMap>
#include <QSqlDatabase>

class MessageRepository {
public:
    explicit MessageRepository(QSqlDatabase db);

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
private:
    QSqlDatabase m_db;
};

#endif // CHATGPT_MESSAGEREPOSITORY_H