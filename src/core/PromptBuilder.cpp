#include "PromptBuilder.h"

QList<Message> PromptBuilder::build(
    const PromptContext& ctx)
{
    QList<Message> result;

    Message system;
    system.role = "system";
    system.content = buildSystem(ctx);

    result.append(system);

    result.append(ctx.history);

    if (!ctx.fileContent.isEmpty())
    {
        Message file;
        file.role = "user";
        file.content =
            "FILE CONTENT:\n\n" +
            ctx.fileContent;

        result.append(file);
    }

    Message user;
    user.role = "user";
    user.content = ctx.userInput;

    result.append(user);

    return result;
}

QString PromptBuilder::buildSystem(const PromptContext& ctx)
{
    QStringList layers;

    // 1. default prompt (hardcoded)
    layers << ctx.defaultSystemPrompt;

    // 2. provider override
    if (!ctx.providerPrompt.isEmpty())
        layers << ctx.providerPrompt;

    // 3. model override
    if (!ctx.modelPrompt.isEmpty())
        layers << ctx.modelPrompt;

    // 4. chat override (strongest)
    if (!ctx.chatPrompt.isEmpty())
        layers << ctx.chatPrompt;

    return layers.join("\n\n");
}

QList<Message> PromptBuilder::buildHistory(const QList<Message>& history)
{
    QList<Message> result;

    for (const auto& msg : history)
    {
        // можно добавить фильтрацию позже (trim tokens)
        result.append(msg);
    }

    return result;
}

QList<Message> PromptBuilder::buildFileContext(const QString& fileContent)
{
    QList<Message> result;

    if (fileContent.isEmpty())
        return result;

    Message fileMsg;
    fileMsg.role = "user";
    fileMsg.content = "FILE:\n\n" + fileContent;

    result.append(fileMsg);

    return result;
}

