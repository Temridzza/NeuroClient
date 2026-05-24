#ifndef CHATGPT_CHATREPOSITORY_H
#define CHATGPT_CHATREPOSITORY_H

#pragma once
#include <QString>
#include <QList>
#include <QVariantMap>

class ChatRepository {
public:
    QString createChat(const QString& title);
    QList<QVariantMap> listChats();
    bool renameChat(const QString& id, const QString& newTitle);
    bool deleteChat(const QString& id);
};



#endif //CHATGPT_CHATREPOSITORY_H