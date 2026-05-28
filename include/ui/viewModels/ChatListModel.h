#ifndef CHATLISTMODEL_H
#define CHATLISTMODEL_H

#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QVariantMap>
#include "ChatRepository.h"

class ChatListModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole
    };

    explicit ChatListModel(ChatRepository* repo, QObject* parent = nullptr);

    // QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // API для QML
    Q_INVOKABLE void refresh();
    Q_INVOKABLE QString createChat(const QString& title);
    Q_INVOKABLE void renameChat(const QString& id, const QString& title);
    Q_INVOKABLE void deleteChat(const QString& id);
    Q_INVOKABLE void openChat(const QString& id);


    Q_INVOKABLE QVariantMap get(int index) const;

private:
    ChatRepository* m_repo = nullptr;
    QList<QVariantMap> m_chats;

signals:
    void chatOpened(const QString& chatId);
};

#endif // CHATLISTMODEL_H
