// LLMClient.h

#ifndef LLMCLIENT_H
#define LLMCLIENT_H

#pragma once

#include <functional>

#include <QString>
#include <QList>

#include "MessageModel.h"

struct LLMConfig {
    QString apiKey;
    QString model;
    QString baseUrl;
};

class LLMClient {
public:
    explicit LLMClient(
        const LLMConfig& config
        );

    void ask(
        const QList<Message>& messages
        );

    QString model() const {
        return m_model;
    }

    std::function<void(const QString&)>
        onStreamChunk;

    std::function<void()>
        onFinished;

    std::function<void(const QString&)>
        onError;

    void setApiKey(QString &apiKey);
    void setModel(QString &model);

private:
    QString m_apiKey;
    QString m_model;
    QString m_baseUrl;
};

#endif // LLMCLIENT_H