#include "ChatViewModel.h"
#include "ChatOrchestrator.h"

ChatViewModel::ChatViewModel(ChatOrchestrator* orchestrator,
    MessageModel* m_messages,
    MessageService* service,
    QObject* parent
    )
    : QObject(parent),
    m_messages(m_messages),
    m_orchestrator(orchestrator),
    m_messageService(service)
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

    QString newChatId = m_orchestrator->sendMessage(
        m_currentChatId,
        text,
        m_messages,
        this
    );

    if (m_currentChatId != newChatId) {
        m_currentChatId = newChatId;

        emit chatIdChanged();
    }
}

void ChatViewModel::loadChat(const QString& chatId)
{
    // qDebug() << "LOAD CHAT" << chatId;
    if (!m_orchestrator)
        return;

    m_currentChatId = chatId;

    m_orchestrator->loadChat(
        chatId,
        m_messages,
        this
        );
    emit chatIdChanged();
    emit messagesChanged();
}

void ChatViewModel::setCurrentChat(const QString& chatId)
{
    if (m_currentChatId == chatId)
        return;

    m_currentChatId = chatId;
    loadChat(chatId);
}

void ChatViewModel::startNewChat()
{
    m_currentChatId.clear();
    m_messages->clear();

    emit chatIdChanged();
    emit messagesChanged();
}

QString ChatViewModel::chatId() const
{
    return m_currentChatId;
}

bool ChatViewModel::hasChat() const
{
    return !m_currentChatId.isEmpty();
}

void ChatViewModel::editMessage(const QString& messageId, const QString& newText)
{
    if (messageId.isEmpty()) {
        qWarning() << "[editMessage] empty messageId";
        return;
    }

    if (newText.trimmed().isEmpty()) {
        qWarning() << "[editMessage] empty newText";
        return;
    }

    // 2. Проверка модели
    if (!m_messages) {
        qWarning() << "[editMessage] m_messages is null";
        return;
    }

    Message old = m_messageService->getMessageById(messageId);

    // 3. Проверка старого сообщения
    if (old.id.isEmpty()) {
        qWarning() << "[editMessage] old message has empty id";
        return;
    }

    qDebug() << "[editMessage] old.id:" << old.id;

    // 4. Fork
    QString newId = m_messageService->forkMessage(old.id, newText);

    qDebug() << "[editMessage] newId:" << newId;

    if (newId.isEmpty()) {
        qWarning() << "[editMessage] forkMessage failed";
        return;
    }

    // 5. Проверка сервиса оркестратора
    if (!m_orchestrator) {
        qWarning() << "[editMessage] orchestrator is null";
        return;
    }

    // 6. Обновление сообщения
    Message updated = old;
    updated.content = newText;
    updated.htmlReady = false;

    m_messages->updateMessageById(messageId, updated);

    qDebug() << "[editMessage] message updated locally";

    // 7. reload chat
    if (!m_currentChatId.isEmpty()) {
        loadChat(m_currentChatId);
    } else {
        qWarning() << "[editMessage] currentChatId is empty";
    }

    // 8. regenerate
    m_orchestrator->regenerateMessage(
        m_currentChatId,
        newId,
        m_messages
        );

    qDebug() << "[editMessage] regeneration triggered";
}
