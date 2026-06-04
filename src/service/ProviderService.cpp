#include "ProviderService.h"

ProviderService::ProviderService(
    ProviderRepository* repository,
    SettingsService* settings
    )
    : m_repo(repository),
    m_settings(settings)
{
}

QList<Provider> ProviderService::providers() const
{
    return m_repo->listProviders();
}

QString ProviderService::currentProviderId() const
{
    return m_settings->currentProviderId();
}

Provider ProviderService::currentProvider(const QString &providerId) const
{
    return m_repo->getProvider(providerId);
}

void ProviderService::setCurrentProvider(const QString& providerId)
{
    if (providerId.isEmpty())
        return;

    m_settings->setCurrentProviderId(providerId);
}

QString ProviderService::defaultProvider() const
{
    const auto providers = m_repo->listProviders();

    if (providers.isEmpty())
        return {};

    return providers.first().id;
}

QString ProviderService::currentApiKey(const QString& providerId) const
{
    const auto provider = currentProvider(providerId);
    return provider.apiKey;
}

QString ProviderService::currentBaseUrl(const QString& providerId) const
{
    const auto provider = currentProvider(providerId);
    return provider.baseUrl;
}

bool ProviderService::updateProvider(const Provider& provider)
{
    if (provider.id.isEmpty())
        return false;

    return m_repo->updateProvider(provider);
}

bool ProviderService::deleteProvider(const QString& id)
{
    return m_repo->deleteProvider(id);
}

QString ProviderService::createProvider(const Provider& provider) {
    return m_repo->createProvider(provider);
}