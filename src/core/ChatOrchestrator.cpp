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
    emit chatLoadingStarted(chatId);

    const QList<Message> messages =
        m_messageService->getMessages(chatId);

    model->setMessages(messages);

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
        m_chatContextService->resolveChatId(
            chatId,
            text
            );

    if (!model || !m_client)
        return {};

    const QString trimmed = text.trimmed();

    if (trimmed.isEmpty())
        return {};

    PromptContext ctx =
        m_chatContextService->buildPromptContext(
            effectiveChatId,
            trimmed
            );

    QList<Message> promptMessages =
        m_promptBuilder->build(ctx);

    // Текущий конец активной ветки
    const QString branchParentId =
        m_messageService->lastMessageId(
            effectiveChatId
            );

    // Создаём user сообщение
    Message userMsg;
    userMsg.chatId = effectiveChatId;
    userMsg.role = "user";
    userMsg.content = trimmed;
    userMsg.parentId = branchParentId;

    const QString userId =
        m_messageService->addMessage(userMsg);

    if (userId.isEmpty()) {
        qWarning() << "Failed to insert user message";
        return {};
    }

    // branchParent -> user
    if (!branchParentId.isEmpty()) {
        m_messageService->setActiveChild(
            branchParentId,
            userId
            );
    }

    // отображаем user в UI
    userMsg.id = userId;

    Message uiMsg;
    uiMsg.chatId = effectiveChatId;
    uiMsg.role = "user";
    uiMsg.content = trimmed;
    uiMsg.id = userId;

    model->addMessage(uiMsg);

    // создаём assistant и запускаем генерацию
    generateReply(
        effectiveChatId,
        userId,
        model,
        promptMessages
        );

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

void ChatOrchestrator::generateReply(
    const QString& chatId,
    const QString& parentMessageId,
    MessageModel* model,
    const QList<Message>& promptMessages
    )
{
    emit requestStarted(chatId);

    Message assistantMsg;
    assistantMsg.chatId = chatId;
    assistantMsg.role = "assistant";
    assistantMsg.parentId = parentMessageId;
    assistantMsg.content.clear();
    assistantMsg.streaming = true;

    const QString assistantId =
        m_messageService->addMessage(assistantMsg);

    if (assistantId.isEmpty()) {
        qWarning() << "Failed to insert assistant placeholder";
        return;
    }

    m_messageService->setActiveChild(
        parentMessageId,
        assistantId
        );

    assistantMsg.id = assistantId;

    model->addMessage(assistantMsg);

    auto buffer =
        std::make_shared<QString>();

    m_client->onStreamChunk =
        [model, buffer](const QString& delta)
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

    m_client->onFinished =
        [this, model, buffer, assistantId, chatId]()
    {
        QMetaObject::invokeMethod(
            model,
            [this, model, buffer, assistantId, chatId]()
            {
                model->finishStreaming();

                if (!buffer->isEmpty()) {
                    m_messageService->updateMessageContent(
                        assistantId,
                        *buffer
                        );
                }

                emit requestFinish(chatId);
            },
            Qt::QueuedConnection
            );
    };

    m_client->onError =
        [this, model, assistantId, chatId](const QString& err)
    {
        QMetaObject::invokeMethod(
            model,
            [this, model, assistantId, chatId, err]()
            {
                model->finishStreaming();

                m_messageService->updateMessageContent(
                    assistantId,
                    QString("[error] %1").arg(err)
                    );

                emit requestFinish(chatId);
            },
            Qt::QueuedConnection
            );
    };

    QtConcurrent::run(
        [this, promptMessages]()
        {
            m_client->ask(promptMessages);
        }
        );
}

void ChatOrchestrator::regenerateMessage(
    const QString& chatId,
    const QString& parentMessageId,
    MessageModel* model
    )
{
    if (!model || !m_client)
        return;

    PromptContext ctx =
        m_chatContextService->buildPromptContext(
            chatId,
            QString()
            );

    QList<Message> promptMessages =
        m_promptBuilder->build(ctx);

    generateReply(
        chatId,
        parentMessageId,
        model,
        promptMessages
        );
}