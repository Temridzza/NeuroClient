#ifndef CHATGPT_DATABASEMANAGER_H
#define CHATGPT_DATABASEMANAGER_H

#pragma once

#include <QString>
#include <QtSql/QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool init(const QString& path = "chat.db");

    QSqlDatabase& db();

private:
    DatabaseManager() = default;

    bool createSchema();

    QSqlDatabase m_db;
};

#endif // CHATGPT_DATABASEMANAGER_H