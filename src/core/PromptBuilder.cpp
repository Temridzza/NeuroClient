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
    // Сначала проверяем самый сильный уровень
    if (!ctx.chatPrompt.isEmpty())
        return ctx.chatPrompt;
    if (!ctx.modelPrompt.isEmpty())
        return ctx.modelPrompt;
    if (!ctx.providerPrompt.isEmpty())
        return ctx.providerPrompt;

    // fallback на дефолт
    return ctx.defaultSystemPrompt;
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

