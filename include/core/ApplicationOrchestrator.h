#ifndef APPLICATIONORCHESTRATOR_H
#define APPLICATIONORCHESTRATOR_H

#pragma once

#include <QObject>

#include "ChatViewModel.h"
#include "ChatListViewModel.h"
#include "ProviderModel.h"
#include "ModelsModel.h"
#include "ModelSelector.h"
#include "ChatOrchestrator.h"
#include "ChatRepository.h"
#include "AppSettings.h"
#include "ModelService.h"
#include "ProviderSelector.h"
#include "ProviderService.h"
#include "LLMClient.h"

class ApplicationOrchestrator : public QObject {
    Q_OBJECT

    Q_PROPERTY(ChatOrchestrator* chatOrchestrator READ chatOrchestrator CONSTANT)
    Q_PROPERTY(SettingsService* settingsService READ settingsService CONSTANT)

    Q_PROPERTY(ChatViewModel* chat READ chat CONSTANT)
    Q_PROPERTY(MessageModel* messages READ messages CONSTANT)

    Q_PROPERTY(ChatListViewModel* chats READ chats CONSTANT)
    Q_PROPERTY(ProviderModel* providers READ providers CONSTANT)
    Q_PROPERTY(ModelsModel* models READ models CONSTANT)
    Q_PROPERTY(ModelSelector* modelSelector READ modelSelector CONSTANT)
    Q_PROPERTY(ProviderSelector* providerSelector READ providerSelector CONSTANT)

public:
    explicit ApplicationOrchestrator(QObject* parent = nullptr);

    void init();
    ChatViewModel* chat();
    MessageModel* messages();

    ChatListViewModel* chats();
    ProviderModel* providers();
    ModelsModel* models();
    ModelSelector* modelSelector();
    ProviderSelector* providerSelector();
    ChatOrchestrator* chatOrchestrator();
    SettingsService* settingsService();

private:
    void rebuildClient();
    QVariantList buildModelList(const QList<QVariantMap>& models) const;

private:
    DatabaseManager m_database;

    std::unique_ptr<AppSettings> m_appSettings;
    std::unique_ptr<SettingsService> m_settingsService;
    std::unique_ptr<ModelService> m_modelService;
    std::unique_ptr<ProviderService> m_providerService;

    std::unique_ptr<LLMClient> m_llmClient;

    std::unique_ptr<ChatRepository> m_chatRepo;
    std::unique_ptr<MessageRepository> m_messageRepo;
    std::unique_ptr<ProviderRepository> m_providerRepo;
    std::unique_ptr<ModelRepository> m_modelRepo;

    std::unique_ptr<ChatOrchestrator> m_orchestrator;

    std::unique_ptr<ModelSelector> m_modelSelector;
    std::unique_ptr<ProviderSelector> m_providerSelector;
    std::unique_ptr<ChatViewModel> m_chatVM;
    std::unique_ptr<MessageModel> m_messagesVM;
    std::unique_ptr<ChatListViewModel> m_chatListVM;
    std::unique_ptr<ProviderModel> m_providerVM;
    std::unique_ptr<ModelsModel> m_modelsVM;
};

#endif // APPLICATIONORCHESTRATOR_H
