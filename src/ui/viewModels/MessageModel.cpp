#include "MessageModel.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent) {

    m_renderTimer.setSingleShot(true);
    m_renderTimer.setInterval(40); // ~25 FPS max render

    connect(&m_renderTimer, &QTimer::timeout,
            this, &MessageModel::renderLastMessage);
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
    case RoleHtmlReady:
        return message.htmlReady;

    default:
        return {};
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const {
    return {
        {RoleRole, "role"},
        {RoleContent, "message"},
        {RoleHtml, "html"},
        {RoleHtmlReady, "htmlReady"}
    };
}

void MessageModel::addMessage(Message msg)
{
    int row = m_messages.size();
    beginInsertRows(QModelIndex(), row, row);
    m_messages.append(msg);
    endInsertRows();
}

void MessageModel::clear() {
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

const QList<Message> &MessageModel::messages() const {
    return m_messages;
}

void MessageModel::startStreaming(const QString &role)
{
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());

    Message msg;
    msg.role = role;
    msg.content = "";
    msg.html = "";
    msg.streaming = true;

    m_messages.append(msg);

    endInsertRows();
}

void MessageModel::appendStream(const QString &delta)
{
    if (m_messages.isEmpty())
        return;

    auto &msg = m_messages.last();
    msg.content += delta;

    // UI обновляем сразу (текст)
    const QModelIndex idx = index(m_messages.size() - 1);

    emit dataChanged(idx, idx, {RoleContent});

    // markdown НЕ рендерим сразу
    scheduleRender();
}

void MessageModel::scheduleRender()
{
    m_renderTimer.start();
}

void MessageModel::renderLastMessage()
{
    if (m_messages.isEmpty())
        return;

    auto &msg = m_messages.last();

    msg.html = markdownToHtml(msg.content);

    const QModelIndex idx = index(m_messages.size() - 1);

    emit dataChanged(idx, idx, {RoleHtml, RoleHtmlReady});
}

void MessageModel::finishStreaming()
{
    if (m_messages.isEmpty())
        return;

    const int row = m_messages.size() - 1;
    auto &msg = m_messages[row];

    msg.streaming = false;
    msg.html = markdownToHtml(msg.content);

    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {RoleHtml, RoleHtmlReady});
}

void MessageModel::ensureRendered(int row)
{
    if (row < 0 || row >= m_messages.size())
        return;

    auto &msg = m_messages[row];

    if (msg.htmlReady)
        return;

    msg.html = markdownToHtml(msg.content);
    msg.htmlReady = true;

    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {RoleHtml, RoleHtmlReady});
}
