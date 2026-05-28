#ifndef CHATGPT_CHATREPOSITORY_H
#define CHATGPT_CHATREPOSITORY_H

#pragma once
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QUuid>
#include <QDateTime>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

#include "database/DatabaseManager.h"

class ChatRepository {
public:
    explicit ChatRepository(QSqlDatabase db);
    QString createChat(const QString& title);
    QList<QVariantMap> listChats();
    bool renameChat(const QString& id, const QString& newTitle);
    bool deleteChat(const QString& id);

private:
    QSqlDatabase m_db;
};



#endif //CHATGPT_CHATREPOSITORY_H