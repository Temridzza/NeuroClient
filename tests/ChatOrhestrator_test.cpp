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

// TEST(ChatOrhestrator, sendMessage) {
//     auto m_db = new DatabaseManager();
//     // QString dbDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     // QDir dir(dbDir);
//     // if (!dir.exists()) {
//     //     dir.mkpath(".");  // создаем директорию если нет
//     // }
//     qDebug() << "Available drivers:" << QSqlDatabase::drivers();
//     qDebug() << "SQLite available:" << QSqlDatabase::isDriverAvailable("QSQLITE");

//     QString dbPath = QDir::temp().filePath("chat_dbTest.db");

//     if (!m_db->init(dbPath)) {
//         qFatal("DB init failed");
//     }

//     auto m_appSettings = new AppSettings();
//     auto m_settingsService = new SettingsService(
//         m_appSettings
//         );

//     auto m_chatRepo = new ChatRepository(m_db->db());
//     auto m_chatService = new ChatService(m_chatRepo);

//     auto m_messageRepo = new MessageRepository(m_db->db());
//     auto m_messageService = new MessageService(m_messageRepo, m_settingsService);

//     auto m_modelRepo = new ModelRepository(m_db->db());
//     auto m_modelService = new ModelService(m_modelRepo, m_settingsService);

//     auto m_providerRepo = new ProviderRepository(m_db->db());
//     auto m_providerService = new ProviderService(m_providerRepo, m_settingsService);

//     auto m_chatContextService = new ChatContextService(
//         m_chatService, m_modelService, m_providerService,
//         m_messageService, m_settingsService
//         );

//     auto m_promptBuilder = new PromptBuilder();

//     auto m_client = new LLMClient(
//         LLMConfig{
//             .apiKey = "",
//             .model = "qwen2.5-coder-1.5b-instruct",
//             .baseUrl = "http://localhost:1234/v1/chat/completions"
//         }
//     );

//     auto m_chatOrhestrator = new ChatOrchestrator(
//         m_chatService, m_messageService, m_chatContextService,
//         m_promptBuilder, m_client
//         );

//     auto provider = new Provider();
//     provider->baseUrl = "testUrl";
//     provider->name = "testName";

//     auto providerId = m_providerRepo->createProvider(*provider);

//     auto model = new Model();
//     model->providerId = providerId;
//     model->modelKey = "testModelKey";
//     model->displayName = "testDisplayName";

//     auto modelId = m_modelRepo->createModel(*model);

//     auto chatId = m_chatRepo->createChat("TestChat", providerId, modelId);
//     auto chat = m_chatService->getChat(chatId);

//     auto message = new MessageModel();

//     auto obj = new QObject();
//     auto str = m_chatOrhestrator->sendMessage(
//         chatId, "hi", message, obj
//         );

//     std::cout << (str).toStdString();
// }