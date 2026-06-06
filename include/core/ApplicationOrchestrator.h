#ifndef APPLICATIONORCHESTRATOR_H
#define APPLICATIONORCHESTRATOR_H

#pragma once

#include <QObject>
#include <QStandardPaths>
#include <QDir>

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
#include "ChatService.h"
#include "MessageService.h"
#include "ChatContextService.h"
#include "ChatSettingsViewModel.h"
#include "ProjectTreeItem.h"
#include "ProjectTreeModel.h"

class ApplicationOrchestrator : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)

    Q_PROPERTY(ChatOrchestrator* chatOrchestrator READ chatOrchestrator CONSTANT)
    Q_PROPERTY(SettingsService* settingsService READ settingsService CONSTANT)

    Q_PROPERTY(ChatViewModel* chat READ chat CONSTANT)
    Q_PROPERTY(MessageModel* messages READ messages CONSTANT)

    Q_PROPERTY(ChatListViewModel* chats READ chats CONSTANT)
    Q_PROPERTY(ProviderModel* providers READ providers CONSTANT)
    Q_PROPERTY(ModelsModel* models READ models CONSTANT)
    Q_PROPERTY(ModelSelector* modelSelector READ modelSelector CONSTANT)
    Q_PROPERTY(ProviderSelector* providerSelector READ providerSelector CONSTANT)

    Q_PROPERTY(ChatSettingsViewModel* chatSettings  READ chatSettings CONSTANT)
    Q_PROPERTY(ProjectTreeModel* projectTreeModel  READ projectTreeModel CONSTANT)

public:
    explicit ApplicationOrchestrator(QObject* parent = nullptr);
    enum CopyMode {
        RawMarkdown,
        PlainText,
        VisibleText
    };
    Q_ENUM(CopyMode)

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
    ChatSettingsViewModel* chatSettings();
    ProjectTreeModel* projectTreeModel();

    bool ready() const { return m_ready; }

signals:
    void readyChanged();
private:
    void rebuildClient();
    QList<Model> buildModelList(const QList<Model>& models) const;
    bool m_ready = false;

private:
    DatabaseManager m_database;

    std::unique_ptr<AppSettings> m_appSettings;

    std::unique_ptr<SettingsService> m_settingsService;
    std::unique_ptr<ChatService> m_chatService;
    std::unique_ptr<ProviderService> m_providerService;
    std::unique_ptr<ModelService> m_modelService;
    std::unique_ptr<MessageService> m_messageService;
    std::unique_ptr<ChatContextService> m_chatContextService;
    std::unique_ptr<ChatSettingsViewModel> m_chatSettingsVM;


    std::unique_ptr<LLMClient> m_llmClient;

    std::unique_ptr<ChatRepository> m_chatRepo;
    std::unique_ptr<MessageRepository> m_messageRepo;
    std::unique_ptr<ProviderRepository> m_providerRepo;
    std::unique_ptr<ModelRepository> m_modelRepo;

    std::unique_ptr<PromptBuilder> m_promptBuilder;

    std::unique_ptr<ChatOrchestrator> m_orchestrator;

    std::unique_ptr<ModelSelector> m_modelSelector;
    std::unique_ptr<ProviderSelector> m_providerSelector;
    std::unique_ptr<ChatViewModel> m_chatVM;
    std::unique_ptr<MessageModel> m_messagesVM;
    std::unique_ptr<ChatListViewModel> m_chatListVM;
    std::unique_ptr<ProviderModel> m_providerVM;
    std::unique_ptr<ModelsModel> m_modelsVM;


    std::unique_ptr<ProjectTreeItem> m_projectTreeItem;
    std::unique_ptr<ProjectTreeModel> m_projectTreeModel;

};

#endif // APPLICATIONORCHESTRATOR_H
