#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#pragma once

#include "ChatRepository.h"

class ChatService {
public:
    explicit ChatService(ChatRepository* repository);

    QString createChat(const QString& title, const QString &providerId, const QString &modelId);

    QList<Chat> listChats() const;

    Chat getChat(const QString& chatId) const;

    bool renameChat(
        const QString& chatId,
        const QString& newTitle
        );

    bool deleteChat(const QString& chatId);

    bool updateChat(const Chat& chat);

private:
    ChatRepository* m_repository;
};

#endif // CHATSERVICE_H
