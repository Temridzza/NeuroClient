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

QString ChatGPTClient::ask(
    const QList<Message>& messages
    ) {
    CURL* curl = curl_easy_init();

    if (!curl) {
        return "Failed to init curl";
    }

    std::string response;

    std::string apiKeyStr =
        apiKey.toStdString();

    std::string modelStr =
        model.toStdString();

    json messagesJson = json::array();

    for (const auto& msg : messages) {
        messagesJson.push_back({
            {"role", msg.role.toStdString()},
            {"content", msg.content.toStdString()}
        });
    }

    json payload = {
        {"model", modelStr},
        {"messages", messagesJson}
    };

    std::string jsonData = payload.dump();

    std::string authHeader =
        "Authorization: Bearer " + apiKeyStr;

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
        );

    headers = curl_slist_append(
        headers,
        authHeader.c_str()
        );

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://openrouter.ai/api/v1/chat/completions"
        );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
        );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        jsonData.c_str()
        );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback
        );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
        );

    CURLcode res =
        curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return QString::fromStdString(
            curl_easy_strerror(res)
            );
    }

    try {
        auto j = json::parse(response);

        return QString::fromStdString(
            j["choices"][0]["message"]["content"]
                .get<std::string>()
            );

    } catch (...) {
        return QString::fromStdString(response);
    }
}



