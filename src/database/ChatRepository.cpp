#include "database/ChatRepository.h"
#include "database/DatabaseManager.h"

#include <QUuid>
#include <QtSql/QSqlQuery>


QString ChatRepository::createChat(const QString &title) {
    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);

    QSqlQuery q(DatabaseManager::instance().db());
    q.prepare("INSERT INTO chats(id, title, created_at, updated_at) "
              "VALUES (?, ?, datetime('now'), datetime('now'))");

    q.addBindValue(id);
    q.addBindValue(title);
    q.exec();

    return id;
}

QList<QVariantMap> ChatRepository::listChats() {
    QList<QVariantMap> list;
    QSqlQuery q(DatabaseManager::instance().db());

    q.exec("SELECT id, title, created_at, updated_at FROM chats ORDER BY updated_at DESC");

    while (q.next()) {
        QVariantMap map;
        map["id"] = q.value("id");
        map["title"] = q.value("title");
        map["created_at"] = q.value("created_at");
        map["updated_at"] = q.value("updated_at");
        list.append(map);
    }

    return list;
}

bool ChatRepository::renameChat(const QString &id, const QString &newTitle) {
    QSqlQuery q(DatabaseManager::instance().db());
    q.prepare("UPDATE chats SET title=?, updated_at=datetime('now') WHERE id=?");
    q.addBindValue(newTitle);
    q.addBindValue(id);
    return q.exec();
}

bool ChatRepository::deleteChat(const QString &id) {
    QSqlQuery q(DatabaseManager::instance().db());
    q.prepare("DELETE FROM messages WHERE chat_id=?");
    q.addBindValue(id);
    q.exec();

    q.prepare("DELETE FROM chats WHERE id=?");
    q.addBindValue(id);
    return q.exec();
}
