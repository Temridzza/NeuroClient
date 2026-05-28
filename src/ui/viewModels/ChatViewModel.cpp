#include "ChatViewModel.h"

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

void ChatViewModel::sendMessage(const QString& text)
{
    if (text.trimmed().isEmpty() || m_loading)
        return;

    auto& settings =
        AppSettings::instance();

    Message msg;
    msg.chatId = m_currentChatId;
    msg.providerId =
        "af47e067-6eeb-4880-9afa-72851ecd96f7";
    msg.modelId =
        "3cd9a462-c130-4be6-8eee-d3f738325105";
    msg.role = "user";
    msg.content = text;


    // передача id в snapshot - должно быть namemodel и nameprovider
    // USER -> DB
    m_messageRepo.addMessage(
        msg.chatId,
        msg.role,
        msg.content,
        msg.providerId,
        msg.modelId,
        msg.providerId,
        msg.modelId
        );

    // USER -> UI
    m_messages.addMessage(
        "user",
        text
        );

    QList<Message> history =
        m_messages.messages();

    setLoading(true);

    m_messages.startStreaming(
        "assistant"
        );

    auto buffer =
        std::make_shared<QString>();

    // STREAM
    m_client->onStreamChunk =
        [this, buffer](const QString& delta)
    {
        QMetaObject::invokeMethod(
            this,
            [this, buffer, delta]()
            {
                *buffer += delta;

                m_messages.appendStream(
                    delta
                    );
            },
            Qt::QueuedConnection
            );
    };

    // FINISH
    m_client->onFinished =
        [this, providerId, modelId, buffer]()
    {
        QMetaObject::invokeMethod(
            this,
            [this, providerId, modelId, buffer]()
            {
                qDebug()
                << "Assistant buffer:"
                << *buffer;

                qDebug()
                    << "Is empty:"
                    << buffer->isEmpty();

                m_messages.finishStreaming();

                if (!buffer->isEmpty()) {
                    m_messageRepo.addMessage(
                        m_currentChatId,
                        "assistant",
                        *buffer,
                        providerId,
                        modelId,
                        providerId,
                        modelId
                        );
                }
                else {
                    qWarning()
                    << "Assistant response is empty";
                }

                setLoading(false);
            },
            Qt::QueuedConnection
            );
    };

    // ERROR
    m_client->onError =
        [this](const QString& err)
    {
        QMetaObject::invokeMethod(
            this,
            [this, err]()
            {
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

    // RUN
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

void ChatViewModel::loadChat(const QString &chatId)
{
    setLoading(true);

    m_messages.clear();

    const auto rows = m_messageRepo.getMessages(chatId);

    for (const auto &row : rows) {      
        Message msg;

        msg.chatId = row.value("chat_id").toString();
        msg.role = row.value("role").toString();
        msg.content = row.value("content").toString();

        msg.providerId = row.value("provider_id").toString();
        msg.modelId = row.value("model_id").toString();
        msg.parentId = row.value("parent_id").toString();

        m_messages.addMessage(msg);
    }

    emit messagesChanged();
    setLoading(false);
}

void ChatViewModel::setCurrentChat(const QString &chatId)
{
    if (m_currentChatId == chatId)
        return;

    m_currentChatId = chatId;

    loadChat(chatId);
}