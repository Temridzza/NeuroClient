#ifndef CHATVIEWMODEL_H
#define CHATVIEWMODEL_H

#pragma once

#include <QObject>
#include "MessageModel.h"

class ChatOrchestrator;

class ChatViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MessageModel* messages READ messages NOTIFY messagesChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    explicit ChatViewModel(ChatOrchestrator* orchestrator, QObject* parent = nullptr);

    MessageModel* messages();

    bool loading() const;
    void setLoading(bool value);

    Q_INVOKABLE void sendMessage(const QString& text);
    Q_INVOKABLE void loadChat(const QString& chatId);
    Q_INVOKABLE void setCurrentChat(const QString& chatId);

signals:
    void messagesChanged();
    void loadingChanged();

private:
    MessageModel m_messages;
    bool m_loading = false;
    QString m_currentChatId;

    ChatOrchestrator* m_orchestrator = nullptr;
};

#endif // CHATVIEWMODEL_H