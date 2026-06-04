#ifndef PROMPTCONTEXT_H
#define PROMPTCONTEXT_H

#include <QString>
#include <QList>

#include "Message.h"

struct PromptContext
{
    QString userInput;

    QList<Message> history;

    QString fileContent;

    QString providerPrompt;
    QString modelPrompt;
    QString chatPrompt;

    QString defaultSystemPrompt;
};

#endif // PROMPTCONTEXT_H
