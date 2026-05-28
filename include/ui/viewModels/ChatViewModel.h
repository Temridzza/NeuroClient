#ifndef CHATVIEWMODEL_H
#define CHATVIEWMODEL_H

#include <QObject>
#include <QThread>
#include <memory>
#include <QtConcurrent/QtConcurrent>

#include "MessageModel.h"
#include "MessageRepository.h"
#include "LLMClient.h"
#include "AppSettings.h"

class ChatViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(MessageModel* messages
                   READ messages
                       CONSTANT)

    Q_PROPERTY(bool loading
                   READ loading
                       NOTIFY loadingChanged)

public:
    explicit ChatViewModel(QObject *parent = nullptr);

    MessageModel* messages();

    bool loading() const;

    Q_INVOKABLE void sendMessage(const QString& text);
    Q_INVOKABLE void loadChat(const QString &chatId);
    Q_INVOKABLE void setCurrentChat(const QString &chatId);

    void rebuildClient();

signals:
    void loadingChanged();
    void messagesChanged();

private:
    MessageModel m_messages;

    bool m_loading = false;

    std::unique_ptr<LLMClient> m_client;

    void setLoading(bool value);
    MessageRepository m_messageRepo;
    QString m_currentChatId;
};

#endif // CHATVIEWMODEL_H