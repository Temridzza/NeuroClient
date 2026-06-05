#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>

#include "ChatOrchestrator.h"
#include "MessageModel.h"

#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QDebug>

#include "ChatOrchestrator.h"
#include "MessageModel.h"
#include "DatabaseManager.h"

TEST(ChatOrhestrator, sendMessage) {
    auto m_db = new DatabaseManager();
    // QString dbDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // QDir dir(dbDir);
    // if (!dir.exists()) {
    //     dir.mkpath(".");  // создаем директорию если нет
    // }
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
    qDebug() << "SQLite available:" << QSqlDatabase::isDriverAvailable("QSQLITE");

    QString dbPath = QDir::home().filePath("Downloads/chat_dbTest.db");

    if (!m_db->init(dbPath)) {
        qFatal("DB init failed");
    }

    auto m_appSettings = new AppSettings();
    auto m_settingsService = new SettingsService(
        m_appSettings
        );

    auto m_chatRepo = new ChatRepository(m_db->db());
    auto m_chatService = new ChatService(m_chatRepo);

    auto m_messageRepo = new MessageRepository(m_db->db());
    auto m_messageService = new MessageService(m_messageRepo, m_settingsService);

    auto m_modelRepo = new ModelRepository(m_db->db());
    auto m_modelService = new ModelService(m_modelRepo, m_settingsService);

    auto m_providerRepo = new ProviderRepository(m_db->db());
    auto m_providerService = new ProviderService(m_providerRepo, m_settingsService);

    auto m_chatContextService = new ChatContextService(
        m_chatService, m_modelService, m_providerService,
        m_messageService, m_settingsService
        );

    auto m_promptBuilder = new PromptBuilder();

    auto m_client = new LLMClient(
        LLMConfig{
            .apiKey = m_providerService->currentApiKey(
                m_chatService->getChat(
                                 m_settingsService->currentChatId()
                                 ).providerId
                ),
            .model = m_modelService->currentModelKey(
                m_chatService->getChat(
                                 m_settingsService->currentChatId()
                                 ).modelId
                ),
            .baseUrl = m_providerService->currentBaseUrl(
                m_chatService->getChat(
                                 m_settingsService->currentChatId()
                                 ).providerId
                )
        }
    );

    auto m_chatOrhestrator = new ChatOrchestrator(
        m_chatService, m_messageService, m_chatContextService,
        m_promptBuilder, m_client
        );

    auto chatId = m_settingsService->currentChatId();
    auto message = new MessageModel();

    auto obj = new QObject();
    auto str = m_chatOrhestrator->sendMessage(
        chatId, "hi", message, obj
        );

    std::cout << (str).toStdString();
}