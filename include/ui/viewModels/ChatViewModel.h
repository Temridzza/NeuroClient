#ifndef CHATVIEWMODEL_H
#define CHATVIEWMODEL_H

#include <QObject>
#include <QThread>

#include "MessageModel.h"
#include "LLMClient.h"

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

signals:
    void loadingChanged();

private:
    MessageModel m_messages;

    bool m_loading = false;

    LLMClient m_client;

    void setLoading(bool value);
};

#endif // CHATVIEWMODEL_H