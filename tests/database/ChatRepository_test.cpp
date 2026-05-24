#include <iostream>
#include <gtest/gtest.h>

#include "database/ChatRepository.h"
#include "../src/database/ChatRepository.cpp"

#include <qcoreapplication.h>

#include "database/DatabaseManager.h"

#include <QString>

TEST(ChatRepository, createChat) {
    int argc = 1;
    char* argv[] = {(char*)"test"};
    QCoreApplication app(argc, argv); // или QApplication для GUI

    DatabaseManager::instance().init("chat_database.db");

    ChatRepository chatRepo;
    QString chatId = chatRepo.createChat("Мой первый чат");

    std::cout << "Создан чат с ID: " << chatId.toStdString();
}