#ifndef CHATORCHESTRATOR_H
#define CHATORCHESTRATOR_H

#pragma once

#include <QObject>
#include <memory>
#include <QSqlDatabase>

#include "LLMClient.h"
#include "Message.h"
#include "MessageRepository.h"
#include "MessageModel.h"
#include "ChatRepository.h"

class ChatOrchestrator : public QObject
{
    Q_OBJECT

public:
    explicit ChatOrchestrator(
        ChatRepository* chatRepo,
        MessageRepository* messageRepo,
        LLMClient* client,
        QObject* parent = nullptr
        );

    void sendMessage(
        const QString& chatId,
        const QString& text,
        MessageModel* model,
        QObject* uiContext
        );

    void loadChat(
        const QString& chatId,
        MessageModel* model,
        QObject* uiContext
        );

    bool renameChat(
        const QString& chatId,
        const QString& title
        );

    bool deleteChat(const QString& chatId);
    QString createChat(const QString& title);
    QList<QVariantMap> chats();

signals:
    void chatLoadingStarted(const QString& chatId);
    void chatLoadingFinished(const QString& chatId);

    void requestStarted(const QString& chatId);
    void requestFinish(const QString& chatId);

private:
    void rebuildClient();

private:
    LLMClient* m_client;

    MessageRepository* m_messageRepo;
    ChatRepository* m_chatRepo;

    QString m_currentChatId;
};

#endif // CHATORCHESTRATOR_H
