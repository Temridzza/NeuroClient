#include "ChatOrchestrator.h"
#include "config/AppSettings.h"

#include <QtConcurrent/QtConcurrent>

ChatOrchestrator::ChatOrchestrator(
    QSqlDatabase db,
    QObject* parent
    )
    : QObject(parent),
    m_messageRepo(db)
{
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

void ChatOrchestrator::sendMessage(
    const QString& chatId,
    const QString& text,
    MessageModel* model,
    QObject* uiContext
    )
{
    if (text.trimmed().isEmpty())
        return;

    Message userMsg;
    userMsg.chatId = chatId;
    userMsg.role = "user";
    userMsg.content = text;

    model->addMessage(userMsg);

    QList<Message> history = model->messages();

    // assistant placeholder
    Message assistantMsg;
    assistantMsg.chatId = chatId;
    assistantMsg.role = "assistant";
    assistantMsg.streaming = true;

    model->addMessage(assistantMsg);

    auto buffer = std::make_shared<QString>();

    m_client->onStreamChunk =
        [model, buffer](const QString& delta)
    {
        QMetaObject::invokeMethod(model, [model, buffer, delta]()
                                  {
                                      *buffer += delta;
                                      model->appendStream(delta);
                                  }, Qt::QueuedConnection);
    };

    m_client->onFinished =
        [this, model, buffer, assistantMsg]()
    {
        QMetaObject::invokeMethod(model, [this, model, buffer, assistantMsg]()
                                  {
                                      model->finishStreaming();

                                      if (!buffer->isEmpty())
                                      {
                                          Message finalMsg = assistantMsg;
                                          finalMsg.content = *buffer;
                                          finalMsg.streaming = false;

                                          m_messageRepo.addMessage(
                                              finalMsg.chatId,
                                              finalMsg.role,
                                              finalMsg.content,
                                              finalMsg.providerId,
                                              finalMsg.modelId,
                                              finalMsg.providerId,
                                              finalMsg.modelId
                                              );
                                      }
                                  }, Qt::QueuedConnection);
    };

    m_client->onError =
        [model](const QString& err)
    {
        QMetaObject::invokeMethod(model, [model, err]()
                                  {
                                      model->finishStreaming();

                                      Message sysMsg;
                                      sysMsg.role = "system";
                                      sysMsg.content = err;

                                      model->addMessage(sysMsg);
                                  }, Qt::QueuedConnection);
    };

    QtConcurrent::run([this, history]()
                      {
                          m_client->ask(history);
                      });
}

void ChatOrchestrator::loadChat(
    const QString& chatId,
    MessageModel* model,
    QObject* uiContext
    )
{
    model->clear();

    const auto rows = m_messageRepo.getMessages(chatId);

    for (const auto& row : rows)
    {
        Message msg;

        msg.chatId = row.value("chat_id").toString();
        msg.role = row.value("role").toString();
        msg.content = row.value("content").toString();

        msg.providerId = row.value("provider_id").toString();
        msg.modelId = row.value("model_id").toString();
        msg.parentId = row.value("parent_id").toString();

        msg.streaming = false;

        model->addMessage(msg);
    }
}

void ChatOrchestrator::rebuildClient()
{
    AppSettings& settings = AppSettings::instance();

    LLMConfig config{
        .apiKey = settings.getApiKey().data.toString(),
        .model = settings.getCurrentModel().data.toString()
    };

    m_client = std::make_unique<LLMClient>(config);
}