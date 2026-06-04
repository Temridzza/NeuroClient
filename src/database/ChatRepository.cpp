#include "database/ChatRepository.h"


ChatRepository::ChatRepository(QSqlDatabase db)
    : m_db(db)
{}

QString ChatRepository::createChat(
    const QString& title,
    const QString& providerId,
    const QString& modelId
    )
{
    const QString id =
        QUuid::createUuid().toString(
            QUuid::WithoutBraces
            );

    const qint64 now =
        QDateTime::currentSecsSinceEpoch();

    QSqlQuery q(m_db);

    q.prepare(R"(
        INSERT INTO chats(
            id,
            title,
            provider_id,
            model_id,
            prompt,
            created_at,
            updated_at
        )
        VALUES(
            :id,
            :title,
            :provider_id,
            :model_id,
            :prompt,
            :created_at,
            :updated_at
        )
    )");

    q.bindValue(":id", id);
    q.bindValue(":title", title);
    q.bindValue(":provider_id", providerId);
    q.bindValue(":model_id", modelId);
    q.bindValue(":prompt", "");
    q.bindValue(":created_at", now);
    q.bindValue(":updated_at", now);

    if (!q.exec()) {
        qWarning()
        << "Failed to create chat:"
        << q.lastError().text();
        return {};
    }

    return id;
}

QList<Chat>
ChatRepository::listChats()
{
    QList<Chat> chats;

    QSqlQuery q(m_db);

    if (!q.exec(R"(
        SELECT *
        FROM chats
        ORDER BY updated_at DESC
    )"))
    {
        qWarning()
        << "listChats failed:"
        << q.lastError().text();

        return chats;
    }

    while (q.next()) {
        chats.append(
            readChat(q)
            );
    }

    return chats;
}

bool ChatRepository::renameChat(
    const QString& id,
    const QString& title
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE chats
        SET
            title = ?,
            updated_at = ?
        WHERE id = ?
    )");

    q.addBindValue(title);

    q.addBindValue(
        QDateTime::currentSecsSinceEpoch()
        );

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning()
        << "renameChat failed:"
        << q.lastError().text();

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool ChatRepository::deleteChat(
    const QString& id
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        DELETE FROM chats
        WHERE id = ?
    )");

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning()
        << "deleteChat failed:"
        << q.lastError().text();

        return false;
    }

    return q.numRowsAffected() > 0;
}

Chat ChatRepository::getChat(
    const QString& id
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT *
        FROM chats
        WHERE id = ?
        LIMIT 1
    )");

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning()
        << "getChat failed:"
        << q.lastError().text();

        return {};
    }

    if (!q.next())
        return {};

    return readChat(q);
}

Chat ChatRepository::readChat(
    const QSqlQuery& q
    )
{
    Chat chat;

    chat.id =
        q.value("id").toString();

    chat.title =
        q.value("title").toString();

    chat.providerId =
        q.value("provider_id").toString();

    chat.modelId =
        q.value("model_id").toString();

    chat.prompt =
        q.value("prompt").toString();

    chat.rootMessageId =
        q.value("root_message_id").toString();

    chat.createdAt =
        q.value("created_at").toLongLong();

    chat.updatedAt =
        q.value("updated_at").toLongLong();

    return chat;
}

bool ChatRepository::updateChat(
    const Chat& chat
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE chats
        SET
            title = ?,
            provider_id = ?,
            model_id = ?,
            prompt = ?,
            root_message_id = ?,
            updated_at = ?
        WHERE id = ?
    )");

    q.addBindValue(chat.title);
    q.addBindValue(chat.providerId);
    q.addBindValue(chat.modelId);
    q.addBindValue(chat.prompt);
    q.addBindValue(chat.rootMessageId);

    q.addBindValue(
        QDateTime::currentSecsSinceEpoch()
        );

    q.addBindValue(chat.id);

    if (!q.exec()) {
        qWarning()
        << "updateChat failed:"
        << q.lastError().text();

        return false;
    }

    return q.numRowsAffected() > 0;
}