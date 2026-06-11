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
    case RoleId:
        return message.id;

    default:
        return {};
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const {
    return {
        {RoleRole, "role"},
        {RoleContent, "message"},
        {RoleHtml, "html"},
        {RoleHtmlReady, "htmlReady"},
        {RoleId, "messageId"}
    };
}

void MessageModel::addMessage(Message msg)
{
    if (msg.id.isEmpty()) {
        qWarning() << "[MessageModel] adding message with EMPTY id!";
    }

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

QString MessageModel::contentAt(int row) const
{
    if (row < 0 || row >= m_messages.size())
        return {};

    return m_messages[row].content;
}

void MessageModel::copyAt(int row, int mode)
{
    if (row < 0 || row >= m_messages.size())
        return;

    const Message &msg = m_messages[row];

    QString result;

    switch (mode) {
    case RawMarkdown:
        result = msg.content;
        break;

    case PlainText: {
        // удаляем markdown грубо, но быстро
        QTextDocument doc;
        doc.setMarkdown(msg.content);
        result = doc.toPlainText();
        break;
    }

    case VisibleText:
        // берём HTML → plain text (максимально "как видно")
        {
            QTextDocument doc;
            doc.setHtml(msg.html);
            result = doc.toPlainText();
        }
        break;
    }

    if (auto *cb = QGuiApplication::clipboard())
        cb->setText(result);
}

Message MessageModel::messageAt(int row) const
{
    if (row < 0 || row >= m_messages.size())
        return {};

    return m_messages[row];
}

void MessageModel::setMessages(const QList<Message>& msgs)
{
    beginResetModel();
    m_messages = msgs;
    endResetModel();
}

void MessageModel::updateMessage(int index, const Message& msg)
{
    if (index < 0 || index >= m_messages.size())
        return;

    m_messages[index] = msg;
    emit dataChanged(
        this->index(index),
        this->index(index),
        { RoleContent, RoleHtml, RoleHtmlReady }
        );
}

void MessageModel::updateMessageById(const QString& id, const Message& msg)
{
    for (int i = 0; i < m_messages.size(); ++i) {
        if (m_messages[i].id == id) {
            m_messages[i] = msg;

            emit dataChanged(
                index(i),
                index(i),
                { RoleContent, RoleHtml, RoleHtmlReady }
                );
            return;
        }
    }
}