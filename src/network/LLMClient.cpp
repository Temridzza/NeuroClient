// LLMClient.cpp

#include "LLMClient.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <sstream>
#include <string>

using json = nlohmann::json;

LLMClient::LLMClient(
    const LLMConfig& config
    )
    : m_apiKey(config.apiKey),
    m_model(config.model),
    m_baseUrl(config.baseUrl)
{
}

static size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp
    ) {
    auto* client =
        static_cast<LLMClient*>(userp);

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

            auto& choice =
                j["choices"][0];

            if (!choice.contains("delta"))
                continue;

            auto& deltaObj =
                choice["delta"];

            if (!deltaObj.contains("content"))
                continue;

            std::string delta =
                deltaObj["content"]
                    .get<std::string>();

            if (client->onStreamChunk) {

                client->onStreamChunk(
                    QString::fromStdString(
                        delta
                        )
                    );
            }

        } catch (...) {
        }
    }

    return size * nmemb;
}

void LLMClient::ask(
    const QList<Message>& messages
    ) {

    // if (m_apiKey.trimmed().isEmpty()) {

    //     if (onError)
    //         onError("API key is empty");

    //     return;
    // }

    if (m_model.trimmed().isEmpty()) {

        if (onError)
            onError("Model is empty");

        return;
    }

    CURL* curl =
        curl_easy_init();

    if (!curl) {

        if (onError)
            onError("Failed to init curl");

        return;
    }

    json messagesJson =
        json::array();

    for (const auto& msg : messages) {

        messagesJson.push_back({
            {
                "role",
                msg.role.toStdString()
            },
            {
                "content",
                msg.content.toStdString()
            }
        });
    }

    json payload = {
        {"model", m_model.toStdString()},
        {"messages", messagesJson},
        {"stream", true}
    };

    std::string jsonData =
        payload.dump();

    std::string authHeader =
        "Authorization: Bearer "
        + m_apiKey.toStdString();

    struct curl_slist* headers =
        nullptr;

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
        m_baseUrl.toStdString().c_str()
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

void LLMClient::setApiKey(QString& apiKey) {
    m_apiKey = apiKey;
}

void LLMClient::setModel(QString& model) {
    m_model = model;
}