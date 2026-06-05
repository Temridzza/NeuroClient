#ifndef CHATSETTINGSVIEWMODEL_H
#define CHATSETTINGSVIEWMODEL_H

#pragma once
#include <QObject>
#include "ChatService.h"
#include "Chat.h"

class ChatSettingsViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString chatId READ chatId NOTIFY chatIdChanged)
    Q_PROPERTY(QString chatTitle READ chatTitle WRITE setChatTitle NOTIFY chatTitleChanged)
    Q_PROPERTY(QString providerId READ providerId WRITE setProviderId NOTIFY providerIdChanged)
    Q_PROPERTY(QString modelId READ modelId WRITE setModelId NOTIFY modelIdChanged)
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt NOTIFY promptChanged)

public:
    explicit ChatSettingsViewModel(ChatService* service, QObject* parent = nullptr)
        : QObject(parent), m_service(service) {}

    // ─── chatId ───────────────────────────────
    QString chatId() const { return m_chat.value("id").toString(); }

    // ─── chatTitle ────────────────────────────
    QString chatTitle() const { return m_chat.value("title").toString(); }
    void setChatTitle(const QString& title) {
        if (m_chat.value("title") != title) {
            m_chat["title"] = title;
            emit chatTitleChanged();
        }
    }

    // ─── providerId ───────────────────────────
    QString providerId() const { return m_chat.value("providerId").toString(); }
    void setProviderId(const QString& id) {
        if (m_chat.value("providerId") != id) {
            m_chat["providerId"] = id;
            emit providerIdChanged();
        }
    }

    // ─── modelId ─────────────────────────────
    QString modelId() const { return m_chat.value("modelId").toString(); }
    void setModelId(const QString& id) {
        if (m_chat.value("modelId") != id) {
            m_chat["modelId"] = id;
            emit modelIdChanged();
        }
    }

    // ─── prompt ──────────────────────────────
    QString prompt() const { return m_chat.value("prompt").toString(); }
    void setPrompt(const QString& text) {
        if (m_chat.value("prompt") != text) {
            m_chat["prompt"] = text;
            emit promptChanged();
        }
    }

    // ─── загрузка чата ───────────────────────
    Q_INVOKABLE void getChatInfo(const QString& id) {
        Chat chat = m_service->getChat(id);
        m_chat = chatToMap(chat);

        emit chatChanged();
        emit chatIdChanged();
        emit chatTitleChanged();
        emit providerIdChanged();
        emit modelIdChanged();
        emit promptChanged();
    }

    // ─── сохранение чата ─────────────────────
    Q_INVOKABLE void saveChat() {
        m_service->updateChat(mapToChat(m_chat));
    }

signals:
    void chatIdChanged();
    void chatChanged();
    void chatTitleChanged();
    void providerIdChanged();
    void modelIdChanged();
    void promptChanged();

private:
    ChatService* m_service;
    QVariantMap m_chat;

    // ─── конвертеры ──────────────────────────
    QVariantMap chatToMap(const Chat& chat) {
        return QVariantMap{
            {"id", chat.id},
            {"title", chat.title},
            {"prompt", chat.prompt},
            {"providerId", chat.providerId},
            {"modelId", chat.modelId},
            {"rootMessageId", chat.rootMessageId},
            {"createdAt", chat.createdAt},
            {"updatedAt", chat.updatedAt}
        };
    }

    Chat mapToChat(const QVariantMap& map) {
        Chat chat;
        chat.id = map.value("id").toString();
        chat.title = map.value("title").toString();
        chat.prompt = map.value("prompt").toString();
        chat.providerId = map.value("providerId").toString();
        chat.modelId = map.value("modelId").toString();
        chat.rootMessageId = map.value("rootMessageId").toString();
        chat.createdAt = map.value("createdAt").toLongLong();
        chat.updatedAt = map.value("updatedAt").toLongLong();
        return chat;
    }
};

#endif // CHATSETTINGSVIEWMODEL_H