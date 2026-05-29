#include "ApplicationOrchestrator.h"

#include <QSqlDatabase>

ApplicationOrchestrator::ApplicationOrchestrator(QObject* parent)
    : QObject(parent),

    m_chatRepo(QSqlDatabase::database()),

    m_chatVM(&m_orchestrator),
    m_chatListVM(&m_chatRepo),

    m_providerVM(QSqlDatabase::database()),
    m_modelsVM(QSqlDatabase::database())
{
}

ChatViewModel* ApplicationOrchestrator::chat() {
    return &m_chatVM;
}

ChatListViewModel* ApplicationOrchestrator::chats() {
    return &m_chatListVM;
}

ProviderModel* ApplicationOrchestrator::providers() {
    return &m_providerVM;
}

ModelsModel* ApplicationOrchestrator::models() {
    return &m_modelsVM;
}

ModelSelector* ApplicationOrchestrator::modelSelector() {
    return &m_modelSelector;
}