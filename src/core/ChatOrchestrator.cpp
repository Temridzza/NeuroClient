#include "ChatOrchestrator.h"
#include <QtConcurrent/QtConcurrent>

ChatOrchestrator::ChatOrchestrator(
    QSqlDatabase db,
    LLMClient *client,
    QObject* parent
    )
    : QObject(parent),
    m_messageRepo(db),
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
        m_messageRepo.getMessages(chatId);

    for (const auto& msg : messages)
    {
        model->addMessage(msg);
    }

    emit chatLoadingFinished(chatId);
}

void ChatOrchestrator::sendMessage(
    const QString& chatId,
    const QString& text,
    MessageModel* model,
    QObject* uiContext
    )
{
    Q_UNUSED(uiContext)

    if (!model || !m_client)
        return;

    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty())
        return;
    emit requestStarted(chatId);

    // 1) Берём tip ветки из БД, до вставки новых сообщений
    const QString branchParentId = m_messageRepo.lastMessageId(chatId);

    // 2) Сохраняем user в БД
    Message userMsg;
    userMsg.chatId = chatId;
    userMsg.role = "user";
    userMsg.content = trimmed;
    userMsg.parentId = branchParentId;

    const QString userId = m_messageRepo.addMessage(userMsg);
    if (userId.isEmpty()) {
        qWarning() << "Failed to insert user message";
        return;
    }

    userMsg.id = userId;
    model->addMessage(userMsg);

    // 3) История для LLM — из БД, уже с user, но ещё без assistant placeholder
    const QList<Message> history = m_messageRepo.getMessages(chatId);

    // 4) Создаём assistant placeholder и сразу сохраняем его в БД
    Message assistantMsg;
    assistantMsg.chatId = chatId;
    assistantMsg.role = "assistant";
    assistantMsg.parentId = userId;
    assistantMsg.content.clear();
    assistantMsg.streaming = true;

    const QString assistantId = m_messageRepo.addMessage(assistantMsg);
    if (assistantId.isEmpty()) {
        qWarning() << "Failed to insert assistant placeholder";
        return;
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

    m_client->onFinished = [this, model, buffer, assistantId, chatId]()
    {
        QMetaObject::invokeMethod(
            model,
            [this, model, buffer, assistantId, chatId]()
            {
                model->finishStreaming();

                if (!buffer->isEmpty()) {
                    m_messageRepo.updateMessageContent(assistantId, *buffer);
                }

                emit requestFinish(chatId);
            },
            Qt::QueuedConnection
            );
    };

    m_client->onError = [this, model, assistantId, chatId](const QString& err)
    {
        QMetaObject::invokeMethod(
            model,
            [this, model, assistantId, chatId, err]()
            {
                model->finishStreaming();

                // Можно оставить assistant-row как failed response
                m_messageRepo.updateMessageContent(
                    assistantId,
                    QString("[error] %1").arg(err)
                    );

                Message sysMsg;
                sysMsg.chatId = chatId;
                sysMsg.role = "system";
                sysMsg.content = err;
                model->addMessage(sysMsg);

                emit requestFinish(chatId);
            },
            Qt::QueuedConnection
            );
    };

    QtConcurrent::run([this, history]()
                      {
                          m_client->ask(history);
                      });
}