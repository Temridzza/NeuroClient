#ifndef CHATGPT_MESSAGEREPOSITORY_H
#define CHATGPT_MESSAGEREPOSITORY_H

#pragma once

#include <QString>
#include <QList>
#include <QVariantMap>
#include <QSqlDatabase>

#include "Message.h"

class MessageRepository {
public:
    explicit MessageRepository(QSqlDatabase db);

    QString addMessage(Message& message);

    QList<Message> getMessages(const QString& chatId);
    QString lastMessageId(const QString& chatId);
    bool updateMessageContent(const QString& id, const QString& content);
    bool setActiveChild(const QString& parentId, const QString& childId);
    Message getMessageById(const QString& id);
    QList<Message> getActiveChain(const QString& chatId);
    QList<Message> getMessagesByParent(const QString& parentId);


private:
    QSqlDatabase m_db;

private:
    Message mapRow(QSqlQuery& q);
};

#endif // CHATGPT_MESSAGEREPOSITORY_H