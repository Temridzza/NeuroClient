#include "ChatGPTClient.h"
#include "ChatGPTConfig.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
// #include <QMessageBox>

#include <QString>
#include <string>
using json = nlohmann::json;

ChatGPTClient::ChatGPTClient() {

    auto apikey = ChatGPTConfig::instance().getApiKey();
    if (!apikey.success) {
        valid = false;
        errorMessage = apikey.error;
        return;
    }

    this->valid = true;
    apiKey = apikey.data.toString();

    auto currentModel = ChatGPTConfig::instance().getCurrentModel();
    if (!currentModel.success) {
        valid = false;
        errorMessage = currentModel.error;
        return;
    }

    this->valid = true;
    model = currentModel.data.toString();

}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

QString ChatGPTClient::ask(const QString& userMessage) {
    CURL* curl = curl_easy_init();
    std::string responseStr;
    std::string apiKey = this->apiKey.toStdString();

    if (curl) {
        // Формируем JSON
        json payload = {
            {"model", model.toStdString()},
            {"messages", {
                {{"role", "user"}, {"content", userMessage.toStdString()}}
            }}
        };

        std::string jsonData = payload.dump();

        // Заголовки
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "HTTP-Referer: https://yourapp.example");
        headers = curl_slist_append(headers, "X-Title: GPTChatExample");

        // Настройки CURL
        curl_easy_setopt(curl, CURLOPT_URL, "https://openrouter.ai/api/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

        // tor (SOCKS5)
        //curl_easy_setopt(curl, CURLOPT_PROXY, "socks5h://127.0.0.1:9050");

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return QString("❌ Ошибка отправки запроса: %1").arg(curl_easy_strerror(res));
        }
    }

    try {
        auto j = json::parse(responseStr);

        if (j.contains("error")) {
            return QString("❌ Ошибка API: %1").arg(
                QString::fromStdString(j["error"]["message"].get<std::string>())
            );
        }

        QString reply = QString::fromStdString(j["choices"][0]["message"]["content"].get<std::string>());
        return reply;

    } catch (const std::exception& e) {
        std::cerr << "Ответ сервера:\n" << responseStr << "\n";
        return QString("Ошибка при разборе JSON: %1").arg(e.what());
    }
}



