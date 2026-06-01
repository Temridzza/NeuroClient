#include "ChatListViewModel.h"

ChatListViewModel::ChatListViewModel(
    ChatOrchestrator *chatOrchestrator,
    QObject* parent
    )
    : QAbstractListModel(parent),
    m_chatOrchestrator(chatOrchestrator)
{
    refresh();
}

int ChatListViewModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;

    return m_chats.size();
}

QVariant ChatListViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_chats.size())
        return {};

    const auto& chat = m_chats.at(index.row());
    switch (role) {
    case IdRole:
        return chat.value("id");
    case TitleRole:
        return chat.value("title");
    default:
        return {};
    }
}

QHash<int, QByteArray> ChatListViewModel::roleNames() const {
    return {
        { IdRole, "id" },
        { TitleRole, "title" }
    };
}

void ChatListViewModel::refresh() {
    beginResetModel();
    m_chats = m_chatOrchestrator->chats();
    endResetModel();
}

// QString ChatListViewModel::createChat(const QString& title) {
//     QString id = m_chatOrchestrator->createChat(title);
//     if (!id.isEmpty()) {
//         refresh();
//     }
//     return id;
// }

void ChatListViewModel::renameChat(const QString& id, const QString& title) {
    if (m_chatOrchestrator->renameChat(id, title)) {
        refresh();
    }
}

void ChatListViewModel::deleteChat(const QString& id) {
    if (m_chatOrchestrator->deleteChat(id)) {
        refresh();
    }
}

QVariantMap ChatListViewModel::get(int index) const {
    if (index < 0 || index >= m_chats.size())
        return {};

    return m_chats.at(index);
}

void ChatListViewModel::openChat(const QString& id)
{
    emit chatOpened(id);
}