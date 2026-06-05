#include "ChatContextService.h"

ChatContextService::ChatContextService(
    ChatService* chatService,
    ModelService* modelService,
    ProviderService* providerService,
    MessageService* messageService,
    SettingsService* settingsService
    )
    : m_chatService(chatService),
    m_modelService(modelService),
    m_providerService(providerService),
    m_messageService(messageService),
    m_settingsService(settingsService)
{}

QString ChatContextService::resolveChatId(
    const QString& chatId,
    const QString& userInput)
{
    if (!chatId.isEmpty())
        return chatId;

    const QString title = generateTitle(userInput);

    auto provideId = m_settingsService->currentProviderId();
    auto modelId = m_settingsService->currentModelId();

    const QString newChatId =
        m_chatService->createChat(title, provideId, modelId);

    if (newChatId.isEmpty()) {
        qWarning() << "Failed to create chat";
        return {};
    }
    m_settingsService->setCurrentChatId(newChatId);
    emit chatCreate(newChatId);
    return newChatId;
}

PromptContext ChatContextService::buildPromptContext(
    const QString& chatId,
    const QString& userInput)
{
    PromptContext ctx;

    const Chat chat =
        m_chatService->getChat(chatId);
    const Model model =
        m_modelService->currentModel(chat.modelId);

    const Provider provider =
        m_providerService->currentProvider(chat.providerId);

    ctx.userInput = userInput;

    ctx.history =
        m_messageService->getMessages(chatId);

    ctx.chatPrompt =
        chat.prompt;

    ctx.modelPrompt =
        model.prompt;

    ctx.providerPrompt =
        provider.prompt;

    ctx.defaultSystemPrompt =
        systemPrompt(chatId);

    return ctx;
}

QString ChatContextService::systemPrompt(const QString& chatId) const
{
    Q_UNUSED(chatId);

    return "You are a helpful assistant. Be concise.";
}

QList<Message> ChatContextService::getHistory(const QString& chatId) const
{
    return m_messageService->getMessages(chatId);
}

QString ChatContextService::generateTitle(const QString& input) const
{
    return input.left(50).trimmed();
}

Chat ChatContextService::getChat(const QString& chatId) const
{
    return m_chatService->getChat(chatId);
}
Model ChatContextService::getCurrentModel(const QString& chatId) const
{
    auto chat = m_chatService->getChat(chatId);
    auto id = chat.modelId;
    if(id.isEmpty()) {
        qWarning() << "Failed to get modelId";
    }
    return m_modelService->currentModel(id);

}
Provider ChatContextService::getCurrentProvider(const QString& chatId) const {
    auto chat = m_chatService->getChat(chatId);
    auto id = chat.providerId;
    if(id.isEmpty()) {
        qWarning() << "Failed to get providerId";
    }
    return m_providerService->currentProvider(id);
}