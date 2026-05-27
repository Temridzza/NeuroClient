// ChatGPTClient.cpp

#include "ChatGPTClient.h"
#include "ChatGPTConfig.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <QString>
#include <QByteArray>

#include <sstream>
#include <string>

using json = nlohmann::json;

ChatGPTClient::ChatGPTClient()
{
    auto apikey = ChatGPTConfig::instance().getApiKey();

    if (!apikey.success) {
        valid = false;
        errorMessage = apikey.error;
        return;
    }

    apiKey = apikey.data.toString();

    auto currentModel =
        ChatGPTConfig::instance().getCurrentModel();

    if (!currentModel.success) {
        valid = false;
        errorMessage = currentModel.error;
        return;
    }

    model = currentModel.data.toString();

    valid = true;
}

static size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
    ) {
    ChatGPTClient* client =
        static_cast<ChatGPTClient*>(userp);

    std::string chunk(
        static_cast<char*>(contents),
        size * nmemb
        );

    std::stringstream stream(chunk);

    std::string line;

    while (std::getline(stream, line)) {

        if (!line.starts_with("data: "))
            continue;

        std::string payload =
            line.substr(6);

        if (payload == "[DONE]")
            continue;

        try {

            auto j = json::parse(payload);

            if (!j.contains("choices"))
                continue;

            auto& choice = j["choices"][0];

            if (!choice.contains("delta"))
                continue;

            auto& deltaObj = choice["delta"];

            if (!deltaObj.contains("content"))
                continue;

            std::string delta =
                deltaObj["content"]
                    .get<std::string>();

            if (client->onStreamChunk) {
                client->onStreamChunk(
                    QString::fromStdString(delta)
                    );
            }

        } catch (...) {
        }
    }

    return size * nmemb;
}

void ChatGPTClient::ask(
    const QList<Message>& messages
    ) {
    if (!valid) {

        if (onError)
            onError(errorMessage);

        return;
    }

    CURL* curl = curl_easy_init();

    if (!curl) {

        if (onError)
            onError("Failed to init curl");

        return;
    }

    json messagesJson = json::array();

    for (const auto& msg : messages) {

        messagesJson.push_back({
            {"role", msg.role.toStdString()},
            {"content", msg.content.toStdString()}
        });
    }

    json payload = {
        {"model", model.toStdString()},
        {"messages", messagesJson},
        {"stream", true}
    };

    std::string jsonData =
        payload.dump();

    std::string authHeader =
        "Authorization: Bearer "
        + apiKey.toStdString();

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
        this
        );

    curl_easy_setopt(
        curl,
        CURLOPT_TIMEOUT,
        0L
        );

    curl_easy_setopt(
        curl,
        CURLOPT_TCP_KEEPALIVE,
        1L
        );

    CURLcode res =
        curl_easy_perform(curl);

    if (res != CURLE_OK) {

        if (onError) {
            onError(
                QString::fromStdString(
                    curl_easy_strerror(res)
                    )
                );
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (onFinished)
        onFinished();
}