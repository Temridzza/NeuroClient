#include "database/MessageRepository.h"
#include "database/DatabaseManager.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QUuid>
#include <QDateTime>
#include <QDebug>

MessageRepository::MessageRepository(QSqlDatabase db)
    : m_db(db)
{
}

QString MessageRepository::addMessage(Message& message)
{
    const QString id =
        QUuid::createUuid().toString(
            QUuid::WithoutBraces
            );

    QSqlQuery q(m_db);

    q.prepare(R"(
        INSERT INTO messages(
            id,
            chat_id,
            parent_id,

            role,
            content,

            provider_id,
            model_id,

            created_at
        )
        VALUES(
            :id,
            :chat_id,
            :parent_id,

            :role,
            :content,

            :provider_id,
            :model_id,

            :created_at
        )
    )");

    q.bindValue(":id", id);
    q.bindValue(":chat_id", message.chatId);

    q.bindValue(
        ":parent_id",
        message.parentId.isEmpty()
            ? QVariant()
            : message.parentId
        );

    q.bindValue(":role", message.role);
    q.bindValue(":content", message.content);

    q.bindValue(
        ":provider_id",
        message.providerId.isEmpty()
            ? QVariant()
            : message.providerId
        );

    q.bindValue(
        ":model_id",
        message.modelId.isEmpty()
            ? QVariant()
            : message.modelId
        );
    q.bindValue(
        ":created_at",
        QDateTime::currentSecsSinceEpoch()
        );

    if (!q.exec()) {
        qWarning() << "SQL:" << q.lastQuery();
        qWarning() << "Error:" << q.lastError();
        qWarning() << "Database error:" << m_db.lastError();

        return {};
    }

    message.id = id;

    return id;
}

QList<Message> MessageRepository::getMessages(const QString& chatId)
{
    QList<Message> list;
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT
            id,
            chat_id,
            parent_id,
            role,
            content,
            provider_id,
            model_id
        FROM messages
        WHERE chat_id = :chat_id
          AND deleted = 0
        ORDER BY created_at ASC, rowid ASC
    )");

    q.bindValue(":chat_id", chatId);

    if (!q.exec()) {
        qWarning() << "Failed to fetch messages:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        Message msg;
        msg.id = q.value("id").toString();
        msg.chatId = q.value("chat_id").toString();
        msg.parentId = q.value("parent_id").toString();
        msg.role = q.value("role").toString();
        msg.content = q.value("content").toString();
        msg.providerId = q.value("provider_id").toString();
        msg.modelId = q.value("model_id").toString();
        list.append(std::move(msg));
    }

    return list;
}

QString MessageRepository::lastMessageId(const QString& chatId)
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT id
        FROM messages
        WHERE chat_id = :chat_id
          AND deleted = 0
        ORDER BY created_at DESC, rowid DESC
        LIMIT 1
    )");

    q.bindValue(":chat_id", chatId);

    if (!q.exec()) {
        qWarning() << "Failed to fetch last message:" << q.lastError().text();
        return {};
    }

    if (!q.next())
        return {};

    return q.value(0).toString();
}

bool MessageRepository::updateMessageContent(
    const QString& id,
    const QString& content
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE messages
        SET content = :content
        WHERE id = :id
    )");

    q.bindValue(":content", content);
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning()
        << "Failed to update message content:"
        << q.lastError().text();
        return false;
    }

    return true;
}