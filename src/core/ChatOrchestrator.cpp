#include "ChatOrchestrator.h"
#include <QtConcurrent/QtConcurrent>

ChatOrchestrator::ChatOrchestrator(
    ChatService *chat,
    MessageService *message,
    ChatContextService *chatContextService,

    PromptBuilder *builder,
    LLMClient *client,
    QObject* parent
    )
    : QObject(parent),
    m_chatService(chat),
    m_messageService(message),
    m_chatContextService(chatContextService),

    m_promptBuilder(builder),
    m_client(client)
{
}

void ChatOrchestrator::loadChat(
    const QString& chatId,
    MessageModel* model,
    QObject* uiContext
    )
{
    Q_UNUSED(uiContext)

    emit chatLoadingStarted(chatId);

    model->clear();

    const QList<Message> messages =
        m_messageService->getMessages(chatId);

    for (const auto& msg : messages)
    {
        model->addMessage(msg);
    }

    emit chatLoadingFinished(chatId);
}

QString ChatOrchestrator::sendMessage(
    const QString& chatId,
    const QString& text,
    MessageModel* model,
    QObject* uiContext
    )
{
    Q_UNUSED(uiContext)

    QString effectiveChatId =
        m_chatContextService->resolveChatId(chatId, text);


    if (!model || !m_client)
        return {};

    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty())
        return {};

    PromptContext ctx =
        m_chatContextService->buildPromptContext(
            effectiveChatId,
            text
            );

    QList<Message> promptMessages =
        m_promptBuilder->build(ctx);

    emit requestStarted(effectiveChatId);

    // Берём tip ветки из БД, до вставки новых сообщений
    const QString branchParentId = m_messageService->lastMessageId(effectiveChatId);

    // Сохраняем user в БД
    Message userMsg;
    userMsg.chatId = effectiveChatId;
    userMsg.role = "user";
    userMsg.content = trimmed;
    userMsg.parentId = branchParentId;

    const QString userId = m_messageService->addMessage(userMsg);
    if (userId.isEmpty()) {
        qWarning() << "Failed to insert user message";
        return {};
    }

    userMsg.id = userId;
    model->startStreaming("user");
    model->appendStream(trimmed);
    model->finishStreaming();

    // Создаём assistant placeholder и сразу сохраняем его в БД
    Message assistantMsg;
    assistantMsg.chatId = effectiveChatId;
    assistantMsg.role = "assistant";
    assistantMsg.parentId = userId;
    assistantMsg.content.clear();
    assistantMsg.streaming = true;

    const QString assistantId = m_messageService->addMessage(assistantMsg);
    if (assistantId.isEmpty()) {
        qWarning() << "Failed to insert assistant placeholder";
        return {};
    }

    assistantMsg.id = assistantId;
    model->addMessage(assistantMsg);

    auto buffer = std::make_shared<QString>();

    m_client->onStreamChunk = [model, buffer](const QString& delta)
    {
        QMetaObject::invokeMethod(
            model,
            [model, buffer, delta]()
            {
                *buffer += delta;
                model->appendStream(delta);
            },
            Qt::QueuedConnection
            );
    };

    m_client->onFinished = [this, model, buffer, assistantId, effectiveChatId]()
    {
        QMetaObject::invokeMethod(
            model,
            [this, model, buffer, assistantId, effectiveChatId]()
            {
                model->finishStreaming();

                if (!buffer->isEmpty()) {
                    m_messageService->updateMessageContent(assistantId, *buffer);
                }

                emit requestFinish(effectiveChatId);
            },
            Qt::QueuedConnection
            );
    };

    m_client->onError = [this, model, assistantId, effectiveChatId](const QString& err)
    {
        QMetaObject::invokeMethod(
            model,
            [this, model, assistantId, effectiveChatId, err]()
            {
                model->finishStreaming();

                m_messageService->updateMessageContent(
                    assistantId,
                    QString("[error] %1").arg(err)
                    );

                Message sysMsg;
                sysMsg.chatId = effectiveChatId;
                sysMsg.role = "system";
                sysMsg.content = err;
                model->addMessage(sysMsg);

                emit requestFinish(effectiveChatId);
            },
            Qt::QueuedConnection
            );
    };

    QtConcurrent::run([this, promptMessages]()
      {
        m_client->ask(promptMessages);
      });

    return effectiveChatId;
}

bool ChatOrchestrator::renameChat(
    const QString& chatId,
    const QString& title
    )
{
    return m_chatService->renameChat(chatId, title);
}

bool ChatOrchestrator::deleteChat(
    const QString& chatId
    )
{
    bool result = m_chatService->deleteChat(chatId);

    if (result) {
        if (m_currentChatId == chatId) {
            m_currentChatId.clear();
            emit hasChatChanged();
        }
    }

    return result;
}

QList<Chat> ChatOrchestrator::chats()
{
    return m_chatService->listChats();
}

bool ChatOrchestrator::hasChat() const
{
    return !m_currentChatId.isEmpty();
}

