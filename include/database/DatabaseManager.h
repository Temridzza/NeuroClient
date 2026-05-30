#ifndef CHATGPT_DATABASEMANAGER_H
#define CHATGPT_DATABASEMANAGER_H

#pragma once

#include <QString>
#include <QtSql/QSqlDatabase>

class DatabaseManager {
public:
    DatabaseManager();

    bool init(const QString& path = "chat.db");

    QSqlDatabase& db();

private:


    bool createSchema();
    bool seed();

    QSqlDatabase m_db;
};

#endif // CHATGPT_DATABASEMANAGER_H