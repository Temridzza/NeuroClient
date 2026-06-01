#ifndef CHATVIEWMODEL_H
#define CHATVIEWMODEL_H

#pragma once

#include <QObject>
#include "MessageModel.h"
#include "ChatOrchestrator.h"

class ChatViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString chatId READ chatId NOTIFY chatIdChanged)
    Q_PROPERTY(bool hasChat READ hasChat NOTIFY chatIdChanged)

public:
    explicit ChatViewModel(ChatOrchestrator* orchestrator,
                           MessageModel* m_messages,
                           QObject* parent = nullptr
                           );

    MessageModel* messages();

    bool loading() const;
    void setLoading(bool value);

    Q_INVOKABLE void sendMessage(const QString& text);
    Q_INVOKABLE void loadChat(const QString& chatId);
    Q_INVOKABLE void setCurrentChat(const QString& chatId);

    Q_INVOKABLE void startNewChat();

    QString chatId() const;
    bool hasChat() const;

signals:
    void messagesChanged();
    void loadingChanged();
    void chatIdChanged();

private:
    MessageModel* m_messages;
    bool m_loading = false;
    QString m_currentChatId;

    ChatOrchestrator* m_orchestrator = nullptr;
};

#endif // CHATVIEWMODEL_H