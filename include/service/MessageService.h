#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#pragma once

#include "MessageRepository.h"
#include "SettingsService.h"

class MessageService {
public:
    explicit MessageService(
        MessageRepository* repository,
        SettingsService* settings
        );

    QString addMessage(Message message);

    QString createUserMessage(
        const QString& chatId,
        const QString& content
        );

    QString createAssistantMessage(
        const QString& chatId,
        const QString& content
        );

    QString createSystemMessage(
        const QString& chatId,
        const QString& content
        );

    QList<Message> getMessages(
        const QString& chatId
        ) const;

    QString lastMessageId(
        const QString& chatId
        ) const;

    bool updateMessageContent(
        const QString& id,
        const QString& content
        );

    QString forkMessage(
        const QString& messageId,
        const QString& newContent
        );

    bool setActiveChild(const QString& parentId,
                        const QString& childId);
    Message getMessageById(const QString& id) const;
    QList<Message> getChildren(
        const QString& parentId) const;

    bool switchSibling(
        const QString& parentId,
        const QString& newActiveChildId
        );
    bool switchToPreviousSibling(const QString& messageId);
    bool switchToNextSibling(const QString& messageId);
    bool hasPreviousSibling(const QString& messageId);
    bool hasNextSibling(const QString& messageId);


private:
    QString createMessage(
        const QString& chatId,
        const QString& role,
        const QString& content
        );
    QList<Message> getSiblings(const QString& messageId) const;


private:
    SettingsService* m_settings;
    MessageRepository* m_repository;
};

#endif // MESSAGESERVICE_H
