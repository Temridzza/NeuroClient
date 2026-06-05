#ifndef CHATSETTINGSVIEWMODEL_H
#define CHATSETTINGSVIEWMODEL_H

#pragma once
#include <QObject>
#include "ChatService.h"
#include "Chat.h"
#include "ProviderSelector.h"
#include "ModelSelector.h"

class ChatSettingsViewModel : public QObject
{
    Q_OBJECT

    // ─── Основные свойства ─────────────────────
    Q_PROPERTY(QString chatId READ chatId NOTIFY chatIdChanged)
    Q_PROPERTY(QString chatTitle READ chatTitle WRITE setChatTitle NOTIFY chatTitleChanged)
    Q_PROPERTY(QString providerId READ providerId WRITE setProviderId NOTIFY providerIdChanged)
    Q_PROPERTY(QString modelId READ modelId WRITE setModelId NOTIFY modelIdChanged)
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt NOTIFY promptChanged)

public:
    explicit ChatSettingsViewModel(
        ChatService* chatService,
        ProviderSelector* providerSelector,
        ModelSelector* modelSelector,
        QObject* parent = nullptr
        )
        : QObject(parent),
        m_chatService(chatService),
        m_providerSelector(providerSelector),
        m_modelSelector(modelSelector)
    {}

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

    // ─── Индексы для ComboBox ─────────────────
    QVariantList providers() const { return m_providers; }
    QVariantList models() const { return m_models; }

    // ─── загрузка чата ───────────────────────
    Q_INVOKABLE void getChatInfo(const QString& id)
    {
        Chat chat = m_chatService->getChat(id);
        m_chat = chatToMap(chat);

        m_providerSelector->setCurrentProvider(chat.providerId);
        m_modelSelector->setCurrentModel(chat.modelId);

        emit chatTitleChanged();
        emit promptChanged();
    }

    // ─── сохранение чата ─────────────────────
    Q_INVOKABLE void saveChat() {
        auto chat = mapToChat(m_chat);
        chat.providerId = m_providerSelector->currentProvider();
        chat.modelId = m_modelSelector->currentModel();
        m_chatService->updateChat(chat);
    }

signals:
    void chatIdChanged();
    void chatChanged();
    void chatTitleChanged();
    void providerIdChanged();
    void modelIdChanged();
    void promptChanged();
    void providerIndexChanged();
    void modelIndexChanged();

private:
    ChatService* m_chatService;
    ProviderSelector* m_providerSelector;
    ModelSelector* m_modelSelector;

    QVariantMap m_chat;

    QVariantList m_providers;
    QVariantList m_models;

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