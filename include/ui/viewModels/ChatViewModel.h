#ifndef CHATVIEWMODEL_H
#define CHATVIEWMODEL_H

#include <QObject>
#include <QThread>
#include <memory>

#include "MessageModel.h"
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

    void rebuildClient();

signals:
    void loadingChanged();

private:
    MessageModel m_messages;

    bool m_loading = false;

    std::unique_ptr<LLMClient> m_client;

    void setLoading(bool value);
};

#endif // CHATVIEWMODEL_H