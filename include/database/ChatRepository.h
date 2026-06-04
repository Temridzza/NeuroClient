#ifndef CHATGPT_CHATREPOSITORY_H
#define CHATGPT_CHATREPOSITORY_H

#pragma once
#include <QString>
#include <QList>
#include <QUuid>
#include <QDateTime>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

#include "DatabaseManager.h"
#include "Chat.h"

class ChatRepository {
public:
    explicit ChatRepository(QSqlDatabase db);
    QString createChat(
        const QString& title,
        const QString& providerId,
        const QString& modelId
        );

    QList<Chat> listChats();
    bool renameChat(const QString& id, const QString& newTitle);
    bool deleteChat(const QString& id);
    Chat getChat(const QString& id);
    bool updateChat(const Chat& chat);


private:
    QSqlDatabase m_db;

    static Chat readChat(
        const QSqlQuery& q
        );
};



#endif //CHATGPT_CHATREPOSITORY_H