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

class ChatOrchestrator : public QObject
{
    Q_OBJECT

public:
    explicit ChatOrchestrator(
        QSqlDatabase db,
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
signals:
    void chatLoadingStarted(const QString& chatId);
    void chatLoadingFinished(const QString& chatId);

    void requestStarted(const QString& chatId);
    void requestFinish(const QString& chatId);

private:
    void rebuildClient();

private:
    LLMClient* m_client;
    MessageRepository m_messageRepo;

    QString m_currentChatId;
};

#endif // CHATORCHESTRATOR_H
