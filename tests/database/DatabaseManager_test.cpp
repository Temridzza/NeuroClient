#include <iostream>
#include <gtest/gtest.h>

#include "database/DatabaseManager.h"
#include "../src/database/DatabaseManager.cpp"

#include <qcoreapplication.h>
#include <QString>

TEST(DatabaseManager, init) {
    // Убедитесь, что QCoreApplication создан
    int argc = 1;
    char* argv[] = {(char*)"test"};
    QCoreApplication app(argc, argv); // или QApplication для GUI

    DatabaseManager::instance().init("chat_database.db");

    ASSERT_TRUE(DatabaseManager::instance().db().isOpen());
}