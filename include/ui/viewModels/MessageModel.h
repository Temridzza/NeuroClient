#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QTimer>

#include "Markdown.h"

struct Message {
    QString id;
    QString chatId;

    QString role;
    QString content;

    QString providerId;
    QString modelId;

    QString parentId;

    bool streaming = false;
};

class MessageModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        RoleRole = Qt::UserRole + 1,
        RoleContent,
        RoleHtml
    };

    explicit MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addMessage(Message msg);

    // 🔥 STREAMING API
    void startStreaming(const QString &role);
    void appendStream(const QString &delta);
    void finishStreaming();

    void clear();

    const QList<Message>& messages() const;

private:
    void scheduleRender();
    void renderLastMessage();
private:
    QList<Message> m_messages;
    QTimer m_renderTimer;
};

#endif // MESSAGEMODEL_H