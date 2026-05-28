#ifndef LLMClient_H
#define LLMClient_H

#pragma once

#include <string>

#include "AppSettings.h"
#include "MessageModel.h"

class LLMClient {
public:
    LLMClient();
    void ask(const QList<Message>& messages);


    bool isValid() const {
        return valid;
    }

    QString lastError() { return errorMessage; }
    Result getModel();

    std::function<void(const QString&)> onStreamChunk;
    std::function<void()> onFinished;
    std::function<void(const QString&)> onError;
private:
    QString apiKey;
    QString model;

    bool valid = false;
    QString errorMessage;
private:
};


#endif //LLMClient_H
