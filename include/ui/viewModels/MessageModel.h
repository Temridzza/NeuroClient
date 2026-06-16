#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QTimer>
#include <QTextDocument>


#include <QGuiApplication>
#include <QClipboard>

#include "Markdown.h"
#include "Message.h"
#include "MessageService.h"

class MessageModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        RoleRole = Qt::UserRole + 1,
        RoleContent,
        RoleHtml,
        RoleHtmlReady,
        RoleId,

        RoleHasPrevSibling,
        RoleHasNextSibling
    };
    Q_ENUM(Roles)

    enum CopyMode {
        RawMarkdown,
        PlainText,
        VisibleText
    };
    Q_ENUM(CopyMode)

    explicit MessageModel(
        MessageService* service,
        QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addMessage(Message msg);
    Q_INVOKABLE void ensureRendered(int row);
    Q_INVOKABLE QString contentAt(int row) const;
    Q_INVOKABLE void copyAt(int row, int mode = RawMarkdown);

    // 🔥 STREAMING API
    void startStreaming(const QString &role);
    void appendStream(const QString &delta);
    void finishStreaming();

    void clear();

    const QList<Message>& messages() const;
    Message messageAt(int row) const;
    void setMessages(const QList<Message>& msgs);
    void updateMessage(int index, const Message& msg);
    void updateMessageById(const QString& id, const Message& msg);
    int indexOfMessage(const QString& id) const;


private:
    void scheduleRender();
    void renderLastMessage();

private:
    MessageService* m_messageService = nullptr;
    QList<Message> m_messages;
    QTimer m_renderTimer;
    QString m_chatId;
};

#endif // MESSAGEMODEL_H