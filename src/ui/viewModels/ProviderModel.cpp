#include "ProviderModel.h"

#include <QDebug>

ProviderModel::ProviderModel(
    ProviderService* m_service,
    QObject* parent
    ): QAbstractListModel(parent),
    m_service(m_service)
{
}

int ProviderModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;

    return m_items.size();
}

QVariant ProviderModel::data(
    const QModelIndex& index,
    int role
    ) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const Provider& provider = m_items[index.row()];

    switch (role) {
    case IdRole:
        return provider.id;

    case NameRole:
        return provider.name;

    case BaseUrlRole:
        return provider.baseUrl;

    case ApiKeyRole:
        return provider.apiKey;

    case CreatedAtRole:
        return provider.createdAt;

    case UpdatedAtRole:
        return provider.updatedAt;

    default:
        return {};
    }
}

QHash<int, QByteArray> ProviderModel::roleNames() const {
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {BaseUrlRole, "baseUrl"},
        {ApiKeyRole, "apiKey"},
        {CreatedAtRole, "createdAt"},
        {UpdatedAtRole, "updatedAt"}
    };
}

void ProviderModel::loadProviders() {
    reloadFromDb();
}

void ProviderModel::reloadFromDb() {
    beginResetModel();
    m_items = m_service->providers();
    endResetModel();
}

QString ProviderModel::createProvider(const QString& name,
                                          const QString& baseUrl,
                                          const QString& apiKey) {
    Provider provider;
    provider.name = name;
    provider.baseUrl = baseUrl;
    provider.apiKey = apiKey;
    QString id = m_service->createProvider(provider);

    if (id.isEmpty())
        return {};

    reloadFromDb();
    return id;
}

bool ProviderModel::updateProvider(
    const QString& id,
    const QVariantMap& fields
    )
{
    Provider provider;

    provider.id = id;
    provider.name = fields.value("name").toString();
    provider.baseUrl = fields.value("baseUrl").toString();
    provider.apiKey = fields.value("apiKey").toString();

    bool ok = m_service->updateProvider(provider);

    if (ok)
        reloadFromDb();

    return ok;
}

bool ProviderModel::deleteProvider(const QString& id) {
    bool ok = m_service->deleteProvider(id);

    if (ok)
        reloadFromDb();

    return ok;
}

// QVariantMap ProviderModel::getProvider(int row) const {
//     if (row < 0 || row >= m_items.size())
//         return {};

//     return m_items[row];
// }

QVariantMap ProviderModel::getProviderById(
    const QString& id
    ) const
{
    for (const auto& provider : m_items)
    {
        if (provider.id == id)
        {
            return {
                {"id", provider.id},
                {"name", provider.name},
                {"baseUrl", provider.baseUrl},
                {"apiKey", provider.apiKey},
                {"createdAt", provider.createdAt},
                {"updatedAt", provider.updatedAt}
            };
        }
    }

    return {};
}