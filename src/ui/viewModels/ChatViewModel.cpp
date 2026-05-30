#include "ChatViewModel.h"
#include "ChatOrchestrator.h"

ChatViewModel::ChatViewModel(ChatOrchestrator* orchestrator,
    MessageModel* m_messages,
    QObject* parent
    )
    : QObject(parent),
    m_messages(m_messages),
    m_orchestrator(orchestrator)
{
    connect(m_orchestrator, &ChatOrchestrator::requestStarted,
            this, [this](const QString&) {
                setLoading(true);
            });

    connect(m_orchestrator, &ChatOrchestrator::requestFinish,
            this, [this](const QString&) {
                setLoading(false);
            });
}

MessageModel* ChatViewModel::messages()
{
    return m_messages;
}

bool ChatViewModel::loading() const
{
    return m_loading;
}

void ChatViewModel::setLoading(bool value)
{
    if (m_loading == value)
        return;

    m_loading = value;
    emit loadingChanged();
}

void ChatViewModel::sendMessage(const QString& text)
{
    if (!m_orchestrator)
        return;

    m_orchestrator->sendMessage(
        m_currentChatId,
        text,
        m_messages,
        this
        );
}

void ChatViewModel::loadChat(const QString& chatId)
{
    if (!m_orchestrator)
        return;

    m_currentChatId = chatId;

    m_orchestrator->loadChat(
        chatId,
        m_messages,
        this
        );
}

void ChatViewModel::setCurrentChat(const QString& chatId)
{
    if (m_currentChatId == chatId)
        return;

    m_currentChatId = chatId;
    loadChat(chatId);
}