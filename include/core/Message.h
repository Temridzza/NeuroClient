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

    QString activeChildId;

    bool htmlReady = false;

    bool streaming = false;
};

#endif // MESSAGE_H