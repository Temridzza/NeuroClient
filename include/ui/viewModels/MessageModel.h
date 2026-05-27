#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include "Markdown.h"

struct Message {
    QString role;
    QString content;
    QString html;
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

    void addMessage(const QString& role,
                    const QString& content);

    void clear();

    QList<Message> messages() const;

private:
    QList<Message> m_messages;
};

#endif // MESSAGEMODEL_H