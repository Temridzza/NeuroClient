#include "ChatViewModel.h"

#include <QtConcurrent/QtConcurrent>

ChatViewModel::ChatViewModel(QObject *parent)
    : QObject(parent) {
    rebuildClient();

    connect(
        &AppSettings::instance(),
        &AppSettings::currentModelChanged,
        this,
        [this](const QString&)
        {
            rebuildClient();
        }
    );
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

void ChatViewModel::sendMessage(const QString &text)
{
    if (text.trimmed().isEmpty())
        return;

    if (m_loading)
        return;

    m_messages.addMessage("user", text);

    QList<Message> history =
        m_messages.messages();

    setLoading(true);

    // 🔥 создаём streaming message
    m_messages.startStreaming("assistant");

    // 🔥 callback на chunk
    m_client->onStreamChunk =
        [this](const QString& delta)
    {
        QMetaObject::invokeMethod(
            this,
            [this, delta]() {

                m_messages.appendStream(delta);

            },
            Qt::QueuedConnection
            );
    };

    // 🔥 finish callback
    m_client->onFinished =
        [this]()
    {
        QMetaObject::invokeMethod(
            this,
            [this]() {

                m_messages.finishStreaming();

                setLoading(false);

            },
            Qt::QueuedConnection
            );
    };

    // 🔥 error callback
    m_client->onError =
        [this](const QString& err)
    {
        QMetaObject::invokeMethod(
            this,
            [this, err]() {

                m_messages.finishStreaming();

                m_messages.addMessage(
                    "system",
                    err
                    );

                setLoading(false);

            },
            Qt::QueuedConnection
            );
    };

    // 🔥 background thread
    QtConcurrent::run(
        [this, history]()
        {
            m_client->ask(history);
        });
}

void ChatViewModel::rebuildClient()
{
    AppSettings& settings =
        AppSettings::instance();

    LLMConfig config {
        .apiKey =
        settings.getApiKey()
            .data.toString(),

        .model =
        settings.getCurrentModel()
            .data.toString()
    };

    m_client =
        std::make_unique<LLMClient>(
            config
            );
}