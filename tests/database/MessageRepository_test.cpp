#include <iostream>
#include <gtest/gtest.h>

#include "database/MessageRepository.h"
#include "../src/database/MessageRepository.cpp"
#include "database/DatabaseManager.h"

#include <qcoreapplication.h>
#include <QString>

TEST(MessageRepository, addMessage) {
    int argc = 1;
    char* argv[] = {(char*)"test"};
    QCoreApplication app(argc, argv); // или QApplication для GUI
    DatabaseManager::instance().init("chat_database.db");


    // Добавляем первое сообщение
    QString chatId = "f63a5d80-71e4-4a9a-91b3-57d287abe2c3";
    // 3. Добавление первого сообщения от пользователя
    MessageRepository messageRepo;
    QString messageId = messageRepo.addMessage(
        chatId,                    // ID чата
        "user",                    // Роль отправителя
        "Привет! Как дела?",       // Текст сообщения
        "gpt-3.5-turbo",          // Модель ИИ
        ""                         // parent_id (пусто для первого сообщения)
    );

    std::cout << "Добавлено сообщение с ID:" << messageId.toStdString();
    // 4. Добавление ответа от ассистента
    QString assistantMessageId = messageRepo.addMessage(
        chatId,
        "assistant",               // Роль ассистента
        "Привет! У меня всё отлично, спасибо! Чем могу помочь?",
        "gpt-3.5-turbo",
        messageId                  // parent_id указывает на предыдущее сообщение
    );

    std::cout << "Добавлен ответ ассистента с ID:" << assistantMessageId.toStdString();
}

TEST(MessageRepository, getMessage) {
    int argc = 1;
    char* argv[] = {(char*)"test"};
    QCoreApplication app(argc, argv); // или QApplication для GUI
    DatabaseManager::instance().init("chat_database.db");

    MessageRepository messageRepo;
    QString chatId = "f63a5d80-71e4-4a9a-91b3-57d287abe2c3";

    QList<QVariantMap> messages = messageRepo.getMessages(chatId);

    std::cout << "\nВсе сообщения в чате:" << std::endl;
    for (const auto& msg : messages) {
        std::cout << "Роль: " << msg["role"].toString().toStdString() << std::endl
                 << "Сообщение: " << msg["content"].toString().toStdString() << std::endl;
    }
}