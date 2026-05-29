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

class ApplicationOrchestrator : public QObject {
    Q_OBJECT

    Q_PROPERTY(ChatViewModel* chat READ chat CONSTANT) //chatVM
    Q_PROPERTY(ChatListViewModel* chats READ chats CONSTANT) //chatModel
    Q_PROPERTY(ProviderModel* providers READ providers CONSTANT)
    Q_PROPERTY(ModelsModel* models READ models CONSTANT)
    Q_PROPERTY(ModelSelector* modelSelector READ modelSelector CONSTANT) //modelVM

public:
    explicit ApplicationOrchestrator(QObject* parent = nullptr);

    ChatViewModel* chat();
    ChatListViewModel* chats();
    ProviderModel* providers();
    ModelsModel* models();
    ModelSelector* modelSelector();

private:
    DatabaseManager m_database;

    ModelSelector m_modelSelector;

    std::unique_ptr<ChatRepository> m_chatRepo;
    std::unique_ptr<ProviderRepository> m_providerRepo;
    std::unique_ptr<ModelRepository> m_modelRepo;

    std::unique_ptr<ChatOrchestrator> m_orchestrator;

    std::unique_ptr<ChatViewModel> m_chatVM;

    std::unique_ptr<ChatListViewModel> m_chatListVM;
    std::unique_ptr<ProviderModel> m_providerVM;
    std::unique_ptr<ModelsModel> m_modelsVM;
};

#endif // APPLICATIONORCHESTRATOR_H
