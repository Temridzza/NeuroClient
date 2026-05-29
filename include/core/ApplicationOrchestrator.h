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

    Q_PROPERTY(ChatViewModel* chat READ chat CONSTANT)
    Q_PROPERTY(ChatListViewModel* chats READ chats CONSTANT)
    Q_PROPERTY(ProviderModel* providers READ providers CONSTANT)
    Q_PROPERTY(ModelsModel* models READ models CONSTANT)
    Q_PROPERTY(ModelSelector* modelSelector READ modelSelector CONSTANT)

public:
    explicit ApplicationOrchestrator(QObject* parent = nullptr);

    ChatViewModel* chat();
    ChatListViewModel* chats();
    ProviderModel* providers();
    ModelsModel* models();
    ModelSelector* modelSelector();

private:
    ChatOrchestrator m_orchestrator;

    ChatRepository m_chatRepo;

    ChatViewModel m_chatVM;
    ChatListViewModel m_chatListVM;

    ProviderModel m_providerVM;
    ModelsModel m_modelsVM;

    ModelSelector m_modelSelector;
};

#endif // APPLICATIONORCHESTRATOR_H
