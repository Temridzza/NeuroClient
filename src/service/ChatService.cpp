#include "ChatService.h"

ChatService::ChatService(
    ChatRepository* repository
    )
    : m_repository(repository)
{
}

QString ChatService::createChat(
    const QString& title,
    const QString& providerId,
    const QString& modelId
    )
{
    const QString finalTitle = title.isEmpty() ? "New Chat" : title;
    return m_repository->createChat(finalTitle, providerId, modelId);
}

QList<Chat> ChatService::listChats() const
{
    return m_repository->listChats();
}

Chat ChatService::getChat(
    const QString& chatId
    ) const
{
    return m_repository->getChat(chatId);
}

bool ChatService::renameChat(
    const QString& chatId,
    const QString& newTitle
    )
{
    if (chatId.isEmpty())
        return false;

    if (newTitle.trimmed().isEmpty())
        return false;

    return m_repository->renameChat(
        chatId,
        newTitle.trimmed()
        );
}

bool ChatService::deleteChat(
    const QString& chatId
    )
{
    if (chatId.isEmpty())
        return false;

    return m_repository->deleteChat(chatId);
}

bool ChatService::updateChat(
    const Chat& chat
    )
{
    if (chat.id.isEmpty())
        return false;

    return m_repository->updateChat(chat);
}