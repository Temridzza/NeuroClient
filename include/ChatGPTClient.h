#ifndef CHATGPTCLIENT_H
#define CHATGPTCLIENT_H

#pragma once

#include <string>

#include "ChatGPTConfig.h"

class ChatGPTClient {
public:
    ChatGPTClient();
    QString ask(const QString& userMessage);


    bool isValid() const {
        return valid;
    }

    QString lastError() { return errorMessage; }
    Result getModel();
private:
    QString apiKey;
    QString model;

    bool valid = false;
    QString errorMessage;
private:
};


#endif //CHATGPTCLIENT_H
