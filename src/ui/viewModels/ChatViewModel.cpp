#include "ChatViewModel.h"
#include "ChatOrchestrator.h"

ChatViewModel::ChatViewModel(ChatOrchestrator* orchestrator, QObject* parent)
    : QObject(parent),
    m_orchestrator(orchestrator)
{
}

MessageModel* ChatViewModel::messages()
{
    return &m_messages;
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

    setLoading(true);

    m_orchestrator->sendMessage(
        m_currentChatId,
        text,
        &m_messages,
        this
        );
}

void ChatViewModel::loadChat(const QString& chatId)
{
    if (!m_orchestrator)
        return;

    m_currentChatId = chatId;

    setLoading(true);

    m_orchestrator->loadChat(
        chatId,
        &m_messages,
        this
        );

    setLoading(false);
}

void ChatViewModel::setCurrentChat(const QString& chatId)
{
    if (m_currentChatId == chatId)
        return;

    m_currentChatId = chatId;
    loadChat(chatId);
}