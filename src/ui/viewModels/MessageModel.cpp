#include "MessageModel.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent) {
}

int MessageModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_messages.size();
}

QVariant MessageModel::data(const QModelIndex &index,
                            int role) const {
    if (!index.isValid())
        return {};

    const auto& message = m_messages[index.row()];

    switch (role) {
    case RoleRole:
        return message.role;

    case RoleContent:
        return message.content;

    case RoleHtml:
        return message.html;

    default:
        return {};
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const {
    return {
        {RoleRole, "role"},
        {RoleContent, "message"},
        {RoleHtml, "html"}
    };
}

void MessageModel::addMessage(const QString &role,
                              const QString &content) {
    QString html = markdownToHtml(content);

    beginInsertRows({}, m_messages.size(), m_messages.size());
    m_messages.append({role, content, html});
    endInsertRows();
}

void MessageModel::clear() {
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

QList<Message> MessageModel::messages() const {
    return m_messages;
}

