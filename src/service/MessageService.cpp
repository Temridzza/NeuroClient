#include "MessageService.h"

MessageService::MessageService(
    MessageRepository* repository
    , SettingsService *settings)
    : m_repository(repository),
    m_settings(settings)
{
}

QString MessageService::addMessage(
    Message message
    )
{
    if (message.chatId.isEmpty())
        return {};

    if (message.role.isEmpty())
        return {};

    return m_repository->addMessage(message);
}

QString MessageService::createMessage(
    const QString& chatId,
    const QString& role,
    const QString& content
    )
{
    if (chatId.isEmpty())
        return {};

    Message message;

    message.chatId = chatId;
    message.role = role;
    message.content = content;

    return m_repository->addMessage(message);
}

QString MessageService::createUserMessage(
    const QString& chatId,
    const QString& content
    )
{
    return createMessage(
        chatId,
        "user",
        content
        );
}

QString MessageService::createAssistantMessage(
    const QString& chatId,
    const QString& content
    )
{
    return createMessage(
        chatId,
        "assistant",
        content
        );
}

QString MessageService::createSystemMessage(
    const QString& chatId,
    const QString& content
    )
{
    return createMessage(
        chatId,
        "system",
        content
        );
}

QList<Message> MessageService::getMessages(
    const QString& chatId
    ) const
{
    return m_repository->getActiveChain(chatId);
}

QString MessageService::lastMessageId(
    const QString& chatId
    ) const
{
    return m_repository->lastMessageId(chatId);
}

bool MessageService::updateMessageContent(
    const QString& id,
    const QString& content
    )
{
    return m_repository->updateMessageContent(
        id,
        content
        );
}

QString MessageService::forkMessage(
    const QString& messageId,
    const QString& newContent
    )
{
    // 1. берём старое сообщение
    Message old = m_repository->getMessageById(messageId);

    if (old.id.isEmpty())
        return {};

    // 2. создаём новое сообщение (fork)
    Message fork;
    fork.chatId = old.chatId;
    fork.role = old.role;
    fork.content = newContent;

    fork.parentId = old.parentId;

    // 3. сохраняем
    QString newId = m_repository->addMessage(fork);

    if (newId.isEmpty())
        return {};

    // 4. переключаем ветку
    if (!old.parentId.isEmpty()) {
        m_repository->setActiveChild(
            old.parentId,
            newId
            );
    }

    return newId;
}

bool MessageService::setActiveChild(const QString &parentId,
                                    const QString &childId) {
    return m_repository->setActiveChild(parentId, childId);
}

Message MessageService::getMessageById(const QString& id) const
{
    if (id.isEmpty())
        return {};

    return m_repository->getMessageById(id);
}

QList<Message> MessageService::getChildren(const QString& parentId) const
{
    return m_repository->getMessagesByParent(parentId);
}

QList<Message> MessageService::getSiblings(const QString& messageId) const
{
    Message msg = m_repository->getMessageById(messageId);

    if (msg.parentId.isEmpty())
        return {};

    return m_repository->getMessagesByParent(msg.parentId);
}

bool MessageService::switchSibling(
    const QString& parentId,
    const QString& newActiveChildId
    )
{
    return m_repository->setActiveChild(parentId, newActiveChildId);
}

bool MessageService::switchToPreviousSibling(const QString& messageId)
{
    Message msg = m_repository->getMessageById(messageId);

    if (msg.id.isEmpty() || msg.parentId.isEmpty())
        return false;

    QList<Message> siblings =
        m_repository->getMessagesByParent(msg.parentId);

    int index = -1;

    for (int i = 0; i < siblings.size(); ++i) {
        if (siblings[i].id == messageId) {
            index = i;
            break;
        }
    }

    if (index <= 0)
        return false;

    const QString newActiveId = siblings[index - 1].id;

    // 🔥 обновляем ветку у родителя
    return m_repository->setActiveChild(msg.parentId, newActiveId);
}

bool MessageService::switchToNextSibling(const QString& messageId)
{
    Message msg = m_repository->getMessageById(messageId);

    if (msg.id.isEmpty() || msg.parentId.isEmpty())
        return false;

    QList<Message> siblings =
        m_repository->getMessagesByParent(msg.parentId);

    int index = -1;

    for (int i = 0; i < siblings.size(); ++i) {
        if (siblings[i].id == messageId) {
            index = i;
            break;
        }
    }

    if (index < 0 || index >= siblings.size() - 1)
        return false;

    const QString newActiveId = siblings[index + 1].id;

    // 🔥 ВАЖНО: меняем activeChildId у parent
    return m_repository->setActiveChild(msg.parentId, newActiveId);
}

bool MessageService::hasPreviousSibling(const QString& messageId)
{
    Message msg = m_repository->getMessageById(messageId);
    if (msg.id.isEmpty() || msg.parentId.isEmpty())
        return false;

    QList<Message> siblings = m_repository->getMessagesByParent(msg.parentId);

    for (int i = 0; i < siblings.size(); ++i) {
        if (siblings[i].id == messageId) {
            return i > 0;
        }
    }

    return false;
}

bool MessageService::hasNextSibling(const QString& messageId)
{
    Message msg = m_repository->getMessageById(messageId);
    if (msg.id.isEmpty() || msg.parentId.isEmpty())
        return false;

    QList<Message> siblings = m_repository->getMessagesByParent(msg.parentId);

    for (int i = 0; i < siblings.size(); ++i) {
        if (siblings[i].id == messageId) {
            return i < siblings.size() - 1;
        }
    }

    return false;
}