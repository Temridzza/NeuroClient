#include "ProviderRepository.h"

ProviderRepository::ProviderRepository(QSqlDatabase db)
    : m_db(std::move(db)) {}

qint64 ProviderRepository::now() const {
    return QDateTime::currentSecsSinceEpoch();
}

QString ProviderRepository::generateId() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString ProviderRepository::createProvider(const QString& name,
                                           const QString& baseUrl,
                                           const QString& apiKey) {
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

    q.addBindValue(id);
    q.addBindValue(name);
    q.addBindValue(baseUrl);
    q.addBindValue(apiKey);
    q.addBindValue(ts);
    q.addBindValue(ts);

    if (!q.exec()) {
        qWarning() << "createProvider failed:" << q.lastError().text();
        return QString();
    }

    return id;
}

bool ProviderRepository::updateProvider(const QString& id,
                                        const QVariantMap& fields) {
    if (fields.isEmpty())
        return false;

    QStringList sets;
    QList<QVariant> values;

    for (auto it = fields.begin(); it != fields.end(); ++it) {
        if (it.key() == "id")
            continue;

        sets << QString("%1 = ?").arg(it.key());
        values << it.value();
    }

    sets << "updated_at = ?";
    values << now();

    QSqlQuery q(m_db);

    QString sql = QString("UPDATE providers SET %1 WHERE id = ?")
                      .arg(sets.join(", "));

    q.prepare(sql);

    for (const auto& v : values)
        q.addBindValue(v);

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "updateProvider failed:" << q.lastError().text();
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

QVariantMap ProviderRepository::getProvider(const QString& id) {
    QSqlQuery q(m_db);
    q.prepare(R"(
        SELECT id, name, base_url, api_key,
               created_at, updated_at
        FROM providers
        WHERE id = ?
    )");

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "getProvider failed:" << q.lastError().text();
        return {};
    }

    if (!q.next())
        return {};

    QVariantMap m;
    m["id"] = q.value("id");
    m["name"] = q.value("name");
    m["base_url"] = q.value("base_url");
    m["api_key"] = q.value("api_key");
    m["created_at"] = q.value("created_at");
    m["updated_at"] = q.value("updated_at");

    return m;
}

QList<QVariantMap> ProviderRepository::listProviders() {
    QList<QVariantMap> list;

    QSqlQuery q(m_db);

    if (!q.exec(R"(
        SELECT id, name, base_url, api_key,
               created_at, updated_at
        FROM providers
        ORDER BY created_at DESC
    )")) {
        qWarning() << "listProviders failed:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value("id");
        m["name"] = q.value("name");
        m["base_url"] = q.value("base_url");
        m["api_key"] = q.value("api_key");
        m["created_at"] = q.value("created_at");
        m["updated_at"] = q.value("updated_at");

        list.push_back(m);
    }

    return list;
}