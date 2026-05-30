#include "ProviderSelector.h"

ProviderSelector::ProviderSelector(
    ProviderService* service,
    QObject *parent
    )
    : QObject(parent),
    m_service(service)
{
    reload();
}

QVariantList ProviderSelector::providers() const
{
    return m_providers;
}

QString ProviderSelector::currentProvider() const
{
    return m_currentProvider;
}

void ProviderSelector::setCurrentProvider(const QString &provider)
{
    if (m_currentProvider == provider)
        return;

    m_currentProvider = provider;

    m_service->setCurrentProvider(provider);

    emit currentProviderChanged();
}

void ProviderSelector::reload()
{
    m_providers.clear();

    const auto providers = m_service->providers();

    for (const auto& p : providers) {
        QVariantMap item;
        item["id"] = p.value("id").toString();
        item["display_name"] = p.value("name").toString();
        m_providers << item;
    }

    const QString saved = m_service->currentProviderId();

    bool found = false;
    for (const auto& p : m_providers) {
        if (p.toMap()["id"].toString() == saved) {
            found = true;
            break;
        }
    }

    m_currentProvider = found && !saved.isEmpty()
                            ? saved
                            : (m_providers.isEmpty()
                                   ? QString()
                                   : m_providers.first().toMap()["id"].toString());

    m_service->setCurrentProvider(m_currentProvider);

    emit providersChanged();
    emit currentProviderChanged();
}

QString ProviderSelector::currentProviderDisplayName() const
{
    for (const auto& p : m_providers) {
        const auto map = p.toMap();
        if (map["id"].toString() == m_currentProvider)
            return map["display_name"].toString();
    }

    return {};
}

int ProviderSelector::currentProviderIndex() const
{
    for (int i = 0; i < m_providers.size(); ++i) {
        const auto map = m_providers[i].toMap();
        if (map.value("id").toString() == m_currentProvider)
            return i;
    }

    return 0;
}