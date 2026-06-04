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
    QVariantList list;
    for (const auto &p : m_providerList) {
        QVariantMap map;
        map["id"] = p.id;
        map["display_name"] = p.name;
        map["baseUrl"] = p.baseUrl;
        map["apiKey"] = p.apiKey;
        map["prompt"] = p.prompt;
        map["createdAt"] = p.createdAt;
        map["updatedAt"] = p.updatedAt;
        list.append(map);
    }
    return list;
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

    if (m_service)
        m_service->setCurrentProvider(provider);

    emit currentProviderChanged(provider);
}

void ProviderSelector::reload()
{
    m_providerList.clear();

    const auto providers = m_service->providers(); // возвращает QList<Provider>

    m_providerList = providers;

    const QString saved = m_service->currentProviderId();

    bool found = false;
    for (const auto &p : m_providerList) {
        if (p.id == saved) {
            found = true;
            break;
        }
    }

    m_currentProvider = found && !saved.isEmpty()
                            ? saved
                            : (m_providerList.isEmpty() ? QString() : m_providerList.first().id);

    if (m_service)
        m_service->setCurrentProvider(m_currentProvider);

    emit providersChanged();
emit currentProviderChanged(m_currentProvider);
}

QString ProviderSelector::currentProviderDisplayName() const
{
    for (const auto &p : m_providerList) {
        if (p.id == m_currentProvider)
            return p.name;
    }
    return {};
}

int ProviderSelector::currentProviderIndex() const
{
    for (int i = 0; i < m_providerList.size(); ++i) {
        if (m_providerList[i].id == m_currentProvider)
            return i;
    }
    return 0;
}