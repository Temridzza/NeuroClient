#ifndef CHATCONTEXTSERVICE_H
#define CHATCONTEXTSERVICE_H

#pragma once

#include <QString>
#include <QList>

#include "Message.h"
#include "Chat.h"
#include "Model.h"
#include "Provider.h"

#include "ChatService.h"
#include "ModelService.h"
#include "ProviderService.h"
#include "MessageService.h"
#include "SettingsService.h"
#include "PromptContext.h"

class ChatContextService : public QObject
{
    Q_OBJECT
public:
    ChatContextService(
        ChatService* chatService,
        ModelService* modelService,
        ProviderService* providerService,
        MessageService* messageService,
        SettingsService* settingsService
        );

    // создаёт чат если нужно и возвращает id
    QString resolveChatId(const QString& chatId, const QString& userInput);

    // основной builder контекста для LLM
    PromptContext buildPromptContext(
        const QString& chatId,
        const QString& userInput
        );

    // отдельные части
    Chat getChat(const QString& chatId) const;
    Model getCurrentModel(const QString& chatId) const;
    Provider getCurrentProvider(const QString& chatId) const;

    QList<Message> getHistory(const QString& chatId) const;

signals:
    void chatCreate(const QString& chatId);

private:
    QString generateTitle(const QString& input) const;
    QString systemPrompt(const QString& chatId) const;

private:
    ChatService* m_chatService;
    ModelService* m_modelService;
    ProviderService* m_providerService;
    MessageService* m_messageService;
    SettingsService* m_settingsService;
};

#endif // CHATCONTEXTSERVICE_H
