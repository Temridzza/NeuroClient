#include "ApplicationOrchestrator.h"

#include <QSqlDatabase>

ApplicationOrchestrator::ApplicationOrchestrator(QObject* parent)
    : QObject(parent)
{
    // Получаем путь к директории для данных приложения
    QString dbDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dbDir);
    if (!dir.exists()) {
        dir.mkpath(".");  // создаем директорию если нет
    }

    QString dbPath = dir.filePath("chat_database.db");

    if (!m_database.init(dbPath)) {
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
    m_messageRepo =
        std::make_unique<MessageRepository>(
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

    m_modelService =
        std::make_unique<ModelService>(
            m_modelRepo.get(),
            m_settingsService.get()
            );
    m_modelSelector =
        std::make_unique<ModelSelector>();

    m_providerService =
        std::make_unique<ProviderService>(
            m_providerRepo.get(),
            m_settingsService.get()
            );
    m_providerSelector =
        std::make_unique<ProviderSelector>(
            m_providerService.get()
            );

    m_llmClient = std::make_unique<LLMClient>(
        LLMConfig{
            .apiKey = m_providerService->currentApiKey(),
            .model = m_modelService->currentModelKey(),
            .baseUrl = m_providerService->currentBaseUrl()
        }
        );

    m_orchestrator =
        std::make_unique<ChatOrchestrator>(
            m_chatRepo.get(),
            m_messageRepo.get(),
            m_llmClient.get()
            );

    m_messagesVM =
        std::make_unique<MessageModel>();

    m_chatVM =
        std::make_unique<ChatViewModel>(
            m_orchestrator.get(),
            m_messagesVM.get()
            );


    m_chatListVM =
        std::make_unique<ChatListViewModel>(
            m_orchestrator.get()
            );

    m_providerVM =
        std::make_unique<ProviderModel>(
            m_providerRepo.get()
            );

    m_modelsVM =
        std::make_unique<ModelsModel>(
            m_modelRepo.get()
            );

    connect(
        m_settingsService.get(),
        &SettingsService::currentModelIdChanged,
        this,
        [this](const QString&)
        {
            auto modelKey = m_modelService->currentModelKey();
            m_llmClient->setModel(modelKey);
        }
        );
    connect(
        m_settingsService.get(),
        &SettingsService::currentProviderIdChanged,
        this,
        [this](const QString&)
        {
            auto providerId = m_settingsService->currentProviderId();
            auto models = m_modelService->modelsByProvider(providerId);

            m_modelSelector->setState(
                buildModelList(models),
                m_modelService->currentModelId()
                );

            rebuildClient();
        }
    );

    connect(
        m_modelSelector.get(),
        &ModelSelector::currentModelChanged,
        this,
        [this]()
        {
            m_modelService->setCurrentModel(
                m_modelSelector->currentModel()
                );
        }
    );

    connect(
        m_orchestrator.get(),
        &ChatOrchestrator::chatLoadingStarted,
        this, [this](const QString& chatId)
        {
            m_settingsService->setCurrentChatId(chatId);
            // qDebug() << "m_settingsService->setCurrentChatId(chatId)"
            //          << chatId;
        }
        );
    init();
}

void ApplicationOrchestrator::init() {
    auto providerId = m_providerService->currentProviderId();

    if (providerId == "-1" || providerId.isEmpty() || providerId.trimmed().isEmpty()) {

        providerId = m_providerService->defaultProvider();
        m_providerService->setCurrentProvider(providerId);
    }

    auto modelId = m_modelService->currentModelId();

    if (modelId == "-1" || modelId.isEmpty() || modelId.trimmed().isEmpty()) {
        m_modelService->setCurrentModel(
            m_modelService->defaultModelForProvider(providerId)
        );
    }

    auto models = m_modelService->modelsByProvider(providerId);
    auto list = buildModelList(models);

    m_modelSelector->setState(list, modelId);

    auto chatId = m_settingsService->currentChatId();
    m_chatVM->loadChat(chatId);
}

QVariantList ApplicationOrchestrator::buildModelList(const QList<QVariantMap>& models) const {

    QVariantList list;

    for (const auto& m : models) {
        QVariantMap item;
        item["id"] = m.value("id").toString();
        item["display_name"] = m.value("display_name").toString();
        list << item;
    }

    return list;
}

void ApplicationOrchestrator::rebuildClient()
{
    // TODO: реализовать загрузку apikey из бд

    LLMConfig config{
        .apiKey = m_providerService->currentApiKey(),
        .model = m_modelService->currentModelKey(),
        .baseUrl = m_providerService->currentBaseUrl()
    };

    m_llmClient = std::make_unique<LLMClient>(config);
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

ProviderSelector* ApplicationOrchestrator::providerSelector() {
    return m_providerSelector.get();
}

MessageModel* ApplicationOrchestrator::messages() {
    return m_messagesVM.get();
}

ChatOrchestrator* ApplicationOrchestrator::chatOrchestrator() {
    return m_orchestrator.get();
}

SettingsService* ApplicationOrchestrator::settingsService() {
    return m_settingsService.get();
}