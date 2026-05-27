#include "ChatViewModel.h"

#include <QtConcurrent/QtConcurrent>

ChatViewModel::ChatViewModel(QObject *parent)
    : QObject(parent) {
}

MessageModel *ChatViewModel::messages() {
    return &m_messages;
}

bool ChatViewModel::loading() const {
    return m_loading;
}

void ChatViewModel::setLoading(bool value) {
    if (m_loading == value)
        return;

    m_loading = value;

    emit loadingChanged();
}

void ChatViewModel::sendMessage(const QString &text) {
    if (text.trimmed().isEmpty())
        return;

    if (m_loading)
        return;

    m_messages.addMessage("user", text);

    QList<Message> history = m_messages.messages();

    setLoading(true);

    QtConcurrent::run([this, history]() {

        QString response = m_client.ask(history);

        QMetaObject::invokeMethod(this, [this, response]() {

            m_messages.addMessage(
                "assistant",
                response
                );

            setLoading(false);

        }, Qt::QueuedConnection);
    });
}