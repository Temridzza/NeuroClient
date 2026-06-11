#ifndef CHATORCHESTRATOR_H
#define CHATORCHESTRATOR_H

#pragma once

#include <QObject>
#include <memory>
#include <QSqlDatabase>

#include "LLMClient.h"
#include "Message.h"
#include "MessageModel.h"
#include "PromptBuilder.h"

#include "ChatService.h"
#include "ProviderService.h"
#include "ModelService.h"
#include "MessageService.h"
#include "ChatContextService.h"

#include "Chat.h"
#include "Model.h"
#include "Provider.h"

class ChatOrchestrator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasChat READ hasChat NOTIFY hasChatChanged)
public:
    explicit ChatOrchestrator(
        ChatService* chatService,
        MessageService* messageService,
        ChatContextService* ChatContextService,

        PromptBuilder* builder,
        LLMClient* client,
        QObject* parent = nullptr
        );

    QString sendMessage(
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
    QList<Chat> chats();

    bool hasChat() const;

    void regenerateMessage(
        const QString& chatId,
        const QString& parentMessageId,
        MessageModel* model
        );
signals:
    void chatLoadingStarted(const QString& chatId);
    void chatLoadingFinished(const QString& chatId);

    void requestStarted(const QString& chatId);
    void requestFinish(const QString& chatId);

    void hasChatChanged();

private:
    void rebuildClient();
    void generateReply(
        const QString& chatId,
        const QString& parentMessageId,
        MessageModel* model,
        const QList<Message>& promptMessages
        );

private:
    ChatService* m_chatService;
    MessageService* m_messageService;

    ChatContextService* m_chatContextService;

    PromptBuilder* m_promptBuilder;

    LLMClient* m_client;

    QString m_currentChatId;
};

#endif // CHATORCHESTRATOR_H
