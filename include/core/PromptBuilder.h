#ifndef PROMPTBUILDER_H
#define PROMPTBUILDER_H

#pragma once

#include <QList>
#include <QString>

#include "Message.h"
#include "PromptContext.h"

struct PromptContext;

class PromptBuilder
{
public:
    QList<Message> build(const PromptContext& ctx);

private:
    QString buildSystem(const PromptContext& ctx);
    QList<Message> buildHistory(const QList<Message>& history);
    QList<Message> buildFileContext(const QString& fileContent);
};

#endif // PROMPTBUILDER_H
