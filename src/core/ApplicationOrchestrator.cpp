#include "ApplicationOrchestrator.h"

#include <QSqlDatabase>

ApplicationOrchestrator::ApplicationOrchestrator(QObject* parent)
    : QObject(parent)
{
    if (!m_database.init("chat_database.db")) {
        qFatal("DB init failed");
    }

    m_chatRepo = std::make_unique<ChatRepository>(m_database.db());
    m_providerRepo = std::make_unique<ProviderRepository>(m_database.db());
    m_modelRepo = std::make_unique<ModelRepository>(m_database.db());

    m_orchestrator = std::make_unique<ChatOrchestrator>(m_database.db());

    m_chatVM = std::make_unique<ChatViewModel>(m_orchestrator.get());
    m_chatListVM = std::make_unique<ChatListViewModel>(m_chatRepo.get());

    m_providerVM = std::make_unique<ProviderModel>(m_providerRepo.get());
    m_modelsVM = std::make_unique<ModelsModel>(m_modelRepo.get());
}

ChatViewModel* ApplicationOrchestrator::chat() {
    return m_chatVM.get();
}

ChatListViewModel* ApplicationOrchestrator::chats() {
    return m_chatListVM.get();
}

ProviderModel* ApplicationOrchestrator::providers() {
    return m_providerVM.get();
}

ModelsModel* ApplicationOrchestrator::models() {
    return m_modelsVM.get();
}

ModelSelector* ApplicationOrchestrator::modelSelector() {
    return &m_modelSelector;
}