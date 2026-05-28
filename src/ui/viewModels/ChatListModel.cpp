#include "ChatListModel.h"

ChatListModel::ChatListModel(ChatRepository* repo, QObject* parent)
    : QAbstractListModel(parent), m_repo(repo) {
    refresh();
}

int ChatListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;

    return m_chats.size();
}

QVariant ChatListModel::data(const QModelIndex& index, int role) const {
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

QHash<int, QByteArray> ChatListModel::roleNames() const {
    return {
        { IdRole, "id" },
        { TitleRole, "title" }
    };
}

void ChatListModel::refresh() {
    beginResetModel();
    m_chats = m_repo->listChats();
    endResetModel();
}

QString ChatListModel::createChat(const QString& title) {
    QString id = m_repo->createChat(title);
    if (!id.isEmpty()) {
        refresh();
    }
    return id;
}

void ChatListModel::renameChat(const QString& id, const QString& title) {
    if (m_repo->renameChat(id, title)) {
        refresh();
    }
}

void ChatListModel::deleteChat(const QString& id) {
    if (m_repo->deleteChat(id)) {
        refresh();
    }
}

QVariantMap ChatListModel::get(int index) const {
    if (index < 0 || index >= m_chats.size())
        return {};

    return m_chats.at(index);
}

void ChatListModel::openChat(const QString& id)
{
    emit chatOpened(id);
}