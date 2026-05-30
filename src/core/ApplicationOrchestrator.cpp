#include "ApplicationOrchestrator.h"

#include <QSqlDatabase>

ApplicationOrchestrator::ApplicationOrchestrator(QObject* parent)
    : QObject(parent)
{
    if (!m_database.init("chat_database.db")) {
        qFatal("DB init failed");
    }

    m_appSettings = std::make_unique<AppSettings>();

    m_settingsService =
        std::make_unique<SettingsService>(
            m_appSettings.get()
            );

    m_chatRepo =
        std::make_unique<ChatRepository>(
            m_database.db()
            );

    m_providerRepo =
        std::make_unique<ProviderRepository>(
            m_database.db()
            );

    m_modelRepo =
        std::make_unique<ModelRepository>(
            m_database.db()
            );

    m_modelSelector =
        std::make_unique<ModelSelector>(
            m_settingsService.get()
            );

    m_orchestrator =
        std::make_unique<ChatOrchestrator>(
            m_database.db(),
            m_settingsService.get()
            );

    m_chatVM =
        std::make_unique<ChatViewModel>(
            m_orchestrator.get()
            );

    m_chatListVM =
        std::make_unique<ChatListViewModel>(
            m_chatRepo.get()
            );

    m_providerVM =
        std::make_unique<ProviderModel>(
            m_providerRepo.get()
            );

    m_modelsVM =
        std::make_unique<ModelsModel>(
            m_modelRepo.get()
            );
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
    return m_modelSelector.get();
}