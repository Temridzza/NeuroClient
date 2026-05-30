#include "ProviderService.h"

ProviderService::ProviderService(
    ProviderRepository* repository,
    SettingsService* settings
    )
    : m_repo(repository),
    m_settings(settings)
{
}

QList<QVariantMap> ProviderService::providers() const
{
    return m_repo->listProviders();
}

QString ProviderService::currentProviderId() const
{
    return m_settings->currentProviderId();
}

QVariantMap ProviderService::currentProvider() const
{
    const QString id = m_settings->currentProviderId();
    if (id.isEmpty())
        return {};

    return m_repo->getProvider(id);
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

    return providers.first().value("id").toString();
}

QString ProviderService::currentApiKey() const
{
    const auto provider = currentProvider();
    return provider.value("api_key").toString();
}

QString ProviderService::currentBaseUrl() const
{
    const auto provider = currentProvider();
    return provider.value("base_url").toString();
}