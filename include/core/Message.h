#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

struct Message {
    QString id;
    QString chatId;

    QString role;
    QString content;

    QString providerId;
    QString modelId;

    QString parentId;
    QString html;

    bool htmlReady;

    bool streaming = false;
};

#endif // MESSAGE_H