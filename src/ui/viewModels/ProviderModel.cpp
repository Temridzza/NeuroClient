#include "ProviderModel.h"

#include <QDebug>

ProviderModel::ProviderModel(
    ProviderRepository* repo,
    QObject* parent
    ): QAbstractListModel(parent),
    m_repo(*repo)
{
}

int ProviderModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;

    return m_items.size();
}

QVariant ProviderModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const auto& item = m_items[index.row()];

    switch (role) {
    case IdRole: return item.value("id");
    case NameRole: return item.value("name");
    case BaseUrlRole: return item.value("base_url");
    case ApiKeyRole: return item.value("api_key");
    case CreatedAtRole: return item.value("created_at");
    case UpdatedAtRole: return item.value("updated_at");
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
    m_items = m_repo.listProviders();
    endResetModel();
}

QString ProviderModel::createProvider(const QString& name,
                                          const QString& baseUrl,
                                          const QString& apiKey) {
    QString id = m_repo.createProvider(name, baseUrl, apiKey);

    if (id.isEmpty())
        return {};

    reloadFromDb();
    return id;
}

bool ProviderModel::updateProvider(const QString& id,
                                       const QVariantMap& fields) {
    bool ok = m_repo.updateProvider(id, fields);

    if (ok)
        reloadFromDb();

    return ok;
}

bool ProviderModel::deleteProvider(const QString& id) {
    bool ok = m_repo.deleteProvider(id);

    if (ok)
        reloadFromDb();

    return ok;
}

QVariantMap ProviderModel::getProvider(int row) const {
    if (row < 0 || row >= m_items.size())
        return {};

    return m_items[row];
}

QVariantMap ProviderModel::getProviderById(
    const QString& id
    ) const {
    for (const auto& item : m_items) {
        if (item.value("id").toString() == id) {
            return item;
        }
    }

    return {};
}