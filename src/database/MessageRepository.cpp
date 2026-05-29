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

QString MessageRepository::addMessage(
    const QString& chatId,
    const QString& role,
    const QString& content,

    const QString& providerId,
    const QString& modelId,

    const QString& providerSnapshot,
    const QString& modelSnapshot,

    const QString& parentId
    ) {
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

            provider_name_snapshot,
            model_name_snapshot,

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

            :provider_snapshot,
            :model_snapshot,

            :created_at
        )
    )");

    q.bindValue(":id", id);

    q.bindValue(":chat_id", chatId);

    q.bindValue(
        ":parent_id",
        parentId.isEmpty()
            ? QVariant()
            : parentId
        );

    q.bindValue(":role", role);
    q.bindValue(":content", content);

    q.bindValue(
        ":provider_id",
        providerId.isEmpty()
            ? QVariant()
            : providerId
        );

    q.bindValue(
        ":model_id",
        modelId.isEmpty()
            ? QVariant()
            : modelId
        );

    q.bindValue(
        ":provider_snapshot",
        providerSnapshot
        );

    q.bindValue(
        ":model_snapshot",
        modelSnapshot
        );

    q.bindValue(
        ":created_at",
        QDateTime::currentSecsSinceEpoch()
        );

    if (!q.exec()) {
        qWarning()
        << "Failed to insert message:"
        << q.lastError().text();

        return {};
    }

    return id;
}

QList<QVariantMap> MessageRepository::getMessages(
    const QString& chatId
    ) {
    QList<QVariantMap> list;

    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT
            id,
            parent_id,

            role,
            content,

            provider_id,
            model_id,

            provider_name_snapshot,
            model_name_snapshot,

            created_at
        FROM messages
        WHERE chat_id = :chat_id
          AND deleted = 0
        ORDER BY created_at ASC
    )");

    q.bindValue(":chat_id", chatId);

    if (!q.exec()) {
        qWarning()
        << "Failed to fetch messages:"
        << q.lastError().text();

        return list;
    }

    while (q.next()) {
        QVariantMap map;

        map["id"] =
            q.value("id");

        map["parent_id"] =
            q.value("parent_id");

        map["role"] =
            q.value("role");

        map["content"] =
            q.value("content");

        map["provider_id"] =
            q.value("provider_id");

        map["model_id"] =
            q.value("model_id");

        map["provider_name"] =
            q.value("provider_name_snapshot");

        map["model_name"] =
            q.value("model_name_snapshot");

        map["created_at"] =
            q.value("created_at");

        list.append(map);
    }

    return list;
}