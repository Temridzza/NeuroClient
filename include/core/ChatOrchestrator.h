#ifndef CHATORCHESTRATOR_H
#define CHATORCHESTRATOR_H

#pragma once

#include <QObject>
#include <memory>

#include "network/LLMClient.h"
#include "core/Message.h"
#include "database/MessageRepository.h"
#include "ui/viewModels/MessageModel.h"

class ChatOrchestrator : public QObject
{
    Q_OBJECT

public:
    explicit ChatOrchestrator(QObject* parent = nullptr);

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

    QString m_currentChatId;
};

#endif // CHATORCHESTRATOR_H
