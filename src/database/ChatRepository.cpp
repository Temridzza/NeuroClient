#include "database/ChatRepository.h"


ChatRepository::ChatRepository(QSqlDatabase db)
    : m_db(db)
{}

QString ChatRepository::createChat(const QString &title) {
    const QString id =
        QUuid::createUuid().toString(QUuid::WithoutBraces);

    const qint64 now =
        QDateTime::currentSecsSinceEpoch();

    QSqlQuery q(m_db);

    q.prepare(R"(
        INSERT INTO chats(
            id,
            title,
            created_at,
            updated_at
        )
        VALUES (
            :id,
            :title,
            :created_at,
            :updated_at
        )
    )");

    q.bindValue(":id", id);
    q.bindValue(":title", title);
    q.bindValue(":created_at", now);
    q.bindValue(":updated_at", now);

    if (!q.exec()) {
        qWarning() << "Failed to create chat:"
                   << q.lastError().text();
        return {};
    }

    return id;
}

QList<QVariantMap> ChatRepository::listChats() {
    QList<QVariantMap> list;

    QSqlQuery q(m_db);

    if (!q.exec(R"(
        SELECT
            id,
            title,
            root_message_id,
            created_at,
            updated_at
        FROM chats
        ORDER BY updated_at DESC
    )")) {
        qWarning() << "listChats failed:"
                   << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap map;

        map["id"] = q.value("id").toString();
        map["title"] = q.value("title").toString();
        map["root_message_id"] = q.value("root_message_id");
        map["created_at"] = q.value("created_at");
        map["updated_at"] = q.value("updated_at");

        list.append(map);
    }

    return list;
}

bool ChatRepository::renameChat(
    const QString &id,
    const QString &newTitle
    ) {
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE chats
        SET
            title = :title,
            updated_at = :updated_at
        WHERE id = :id
    )");

    q.bindValue(":title", newTitle);
    q.bindValue(":updated_at", QDateTime::currentSecsSinceEpoch());
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "renameChat failed:"
                   << q.lastError().text();
        return false;
    }

    return true;
}

bool ChatRepository::deleteChat(const QString &id) {
    QSqlQuery q(m_db);

    q.prepare(R"(
        DELETE FROM chats
        WHERE id = :id
    )");

    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "deleteChat failed:"
                   << q.lastError().text();
        return false;
    }

    return true;
}