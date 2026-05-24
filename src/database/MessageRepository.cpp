#include "database/MessageRepository.h"
#include "database/DatabaseManager.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QUuid>

QString MessageRepository::addMessage(
        const QString &chatId,
        const QString &role,
        const QString &content,
        const QString &model,
        const QString &parentId
    )
{
    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);

    QSqlQuery q(DatabaseManager::instance().db());
    q.prepare("INSERT INTO messages(id, chat_id, role, content, model_used, parent_id, created_at) "
              "VALUES (?, ?, ?, ?, ?, ?, datetime('now'))");
    q.addBindValue(id);
    q.addBindValue(chatId);
    q.addBindValue(role);
    q.addBindValue(content);
    q.addBindValue(model);
    q.addBindValue(parentId);

    q.exec();

    return id;
}

QList<QVariantMap> MessageRepository::getMessages(const QString &chatId) {
    QList<QVariantMap> list;

    QSqlQuery q(DatabaseManager::instance().db());
    q.prepare("SELECT * FROM messages WHERE chat_id = ? ORDER BY created_at ASC");
    q.addBindValue(chatId);
    q.exec();

    while (q.next()) {
        QVariantMap map;
        map["id"] = q.value("id");
        map["role"] = q.value("role");
        map["content"] = q.value("content");
        map["model_used"] = q.value("model_used");
        map["parent_id"] = q.value("parent_id");
        map["created_at"] = q.value("created_at");
        list.append(map);
    }

    return list;
}
