#include "database/DatabaseManager.h"
#include <QtSql/QSqlError>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

bool DatabaseManager::init(const QString &path) {
    if (!QSqlDatabase::contains("chat_connection")) {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "chat_connection");
        m_db.setDatabaseName(path);
    } else {
        m_db = QSqlDatabase::database("chat_connection");
    }

    if (!m_db.open()) {
        qDebug() << "DB open error:" << m_db.lastError().text();
        return false;
    }

    runMigrations();
    return true;
}

QSqlDatabase& DatabaseManager::db() {
    return m_db;
}

void DatabaseManager::runMigrations() {
    QSqlQuery q(m_db);

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS chats (
            id TEXT PRIMARY KEY,
            title TEXT,
            created_at TEXT,
            updated_at TEXT
        )
    )");

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS messages (
            id TEXT PRIMARY KEY,
            chat_id TEXT,
            role TEXT,
            content TEXT,
            model_used TEXT,
            parent_id TEXT,
            created_at TEXT,

            FOREIGN KEY(chat_id) REFERENCES chats(id)
        )
    )");
}
