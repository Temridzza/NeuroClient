#include "ProviderRepository.h"

ProviderRepository::ProviderRepository(QSqlDatabase db)
    : m_db(std::move(db)) {}

qint64 ProviderRepository::now() const {
    return QDateTime::currentSecsSinceEpoch();
}

QString ProviderRepository::generateId() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString ProviderRepository::createProvider(const Provider& provider) {
    QString id = generateId();
    qint64 ts = now();

    QSqlQuery q(m_db);
    q.prepare(R"(
        INSERT INTO providers (
            id, name, base_url, api_key,
            created_at, updated_at
        )
        VALUES (?, ?, ?, ?, ?, ?)
    )");

    q.addBindValue(provider.id);
    q.addBindValue(provider.name);
    q.addBindValue(provider.baseUrl);
    q.addBindValue(provider.apiKey);
    q.addBindValue(ts);
    q.addBindValue(ts);

    if (!q.exec()) {
        qWarning() << "createProvider failed:" << q.lastError().text();
        return QString();
    }

    return id;
}

bool ProviderRepository::updateProvider(
    const Provider& provider
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE providers
        SET
            name = ?,
            base_url = ?,
            api_key = ?,
            prompt = ?,
            updated_at = ?
        WHERE id = ?
    )");

    q.addBindValue(provider.name);
    q.addBindValue(provider.baseUrl);
    q.addBindValue(provider.apiKey);
    q.addBindValue(provider.prompt);
    q.addBindValue(now());
    q.addBindValue(provider.id);

    if (!q.exec()) {
        qWarning()
        << "updateProvider failed:"
        << q.lastError().text();

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool ProviderRepository::deleteProvider(const QString& id) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM providers WHERE id = ?");
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "deleteProvider failed:" << q.lastError().text();
        return false;
    }

    return q.numRowsAffected() > 0;
}

Provider ProviderRepository::getProvider(
    const QString& id
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT
            id,
            name,
            base_url,
            api_key,
            prompt,
            created_at,
            updated_at
        FROM providers
        WHERE id = ?
    )");

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning()
        << "getProvider failed:"
        << q.lastError().text();

        return {};
    }

    if (!q.next())
        return {};

    Provider provider;

    provider.id =
        q.value("id").toString();

    provider.name =
        q.value("name").toString();

    provider.baseUrl =
        q.value("base_url").toString();

    provider.apiKey =
        q.value("api_key").toString();

    provider.prompt =
        q.value("prompt").toString();

    provider.createdAt =
        q.value("created_at").toLongLong();

    provider.updatedAt =
        q.value("updated_at").toLongLong();

    return provider;
}

QList<Provider>
ProviderRepository::listProviders()
{
    QList<Provider> providers;

    QSqlQuery q(m_db);

    if (!q.exec(R"(
        SELECT
            id,
            name,
            base_url,
            api_key,
            prompt,
            created_at,
            updated_at
        FROM providers
        ORDER BY created_at DESC
    )"))
    {
        qWarning()
        << "listProviders failed:"
        << q.lastError().text();

        return providers;
    }

    while (q.next())
    {
        Provider provider;

        provider.id =
            q.value("id").toString();

        provider.name =
            q.value("name").toString();

        provider.baseUrl =
            q.value("base_url").toString();

        provider.apiKey =
            q.value("api_key").toString();

        provider.prompt =
            q.value("prompt").toString();

        provider.createdAt =
            q.value("created_at").toLongLong();

        provider.updatedAt =
            q.value("updated_at").toLongLong();

        providers.append(provider);
    }

    return providers;
}