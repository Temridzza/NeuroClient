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
#include "SettingsService.h"

class ChatOrchestrator : public QObject
{
    Q_OBJECT

public:
    explicit ChatOrchestrator(
        QSqlDatabase db,
        SettingsService* settings,
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

private:
    void rebuildClient();

private:
    std::unique_ptr<LLMClient> m_client;
    MessageRepository m_messageRepo;
    SettingsService* m_settings;

    QString m_currentChatId;
};

#endif // CHATORCHESTRATOR_H
