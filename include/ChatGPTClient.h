#ifndef CHATGPTCLIENT_H
#define CHATGPTCLIENT_H

#pragma once

#include <string>

#include "ChatGPTConfig.h"
#include "MessageModel.h"

class ChatGPTClient {
public:
    ChatGPTClient();
    QString ask(const QList<Message>& messages);


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
