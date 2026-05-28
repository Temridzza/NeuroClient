#include "ModelRepository.h"

ModelRepository::ModelRepository(QSqlDatabase db)
    : m_db(db) {}

QString ModelRepository::createModel(
    const QString& providerId,
    const QString& modelKey,
    const QString& displayName,
    const QString& apiKeyOverride,
    int contextWindow,
    int maxOutputTokens,
    bool enabled
    ) {
    const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const qint64 now = QDateTime::currentSecsSinceEpoch();

    QSqlQuery q(m_db);

    q.prepare(R"(
        INSERT INTO models (
            id,
            provider_id,
            model_key,
            display_name,
            api_key_override,
            context_window,
            max_output_tokens,
            enabled,
            created_at,
            updated_at
        )
        VALUES (
            :id,
            :provider_id,
            :model_key,
            :display_name,
            :api_key_override,
            :context_window,
            :max_output_tokens,
            :enabled,
            :created_at,
            :updated_at
        )
    )");

    q.bindValue(":id", id);
    q.bindValue(":provider_id", providerId);
    q.bindValue(":model_key", modelKey);
    q.bindValue(":display_name", displayName);
    q.bindValue(":api_key_override", apiKeyOverride);
    q.bindValue(":context_window", contextWindow);
    q.bindValue(":max_output_tokens", maxOutputTokens);
    q.bindValue(":enabled", enabled ? 1 : 0);
    q.bindValue(":created_at", now);
    q.bindValue(":updated_at", now);

    if (!q.exec()) {
        qWarning() << "createModel failed:" << q.lastError().text();
        return {};
    }

    return id;
}

QList<QVariantMap> ModelRepository::listModels() const {
    QList<QVariantMap> list;

    QSqlQuery q(m_db);

    if (!q.exec(R"(
        SELECT
            id,
            provider_id,
            model_key,
            display_name,
            api_key_override,
            context_window,
            max_output_tokens,
            enabled,
            created_at,
            updated_at
        FROM models
        ORDER BY display_name ASC
    )")) {
        qWarning() << "listModels failed:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap item;

        item["id"] = q.value("id");
        item["provider_id"] = q.value("provider_id");
        item["model_key"] = q.value("model_key");
        item["display_name"] = q.value("display_name");
        item["api_key_override"] = q.value("api_key_override");
        item["context_window"] = q.value("context_window");
        item["max_output_tokens"] = q.value("max_output_tokens");
        item["enabled"] = q.value("enabled").toBool();
        item["created_at"] = q.value("created_at");
        item["updated_at"] = q.value("updated_at");

        list.append(item);
    }

    return list;
}

QList<QVariantMap> ModelRepository::listModelsByProvider(const QString& providerId) const {
    QList<QVariantMap> list;

    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT
            id,
            provider_id,
            model_key,
            display_name,
            api_key_override,
            context_window,
            max_output_tokens,
            enabled,
            created_at,
            updated_at
        FROM models
        WHERE provider_id = :provider_id
        ORDER BY display_name ASC
    )");

    q.bindValue(":provider_id", providerId);

    if (!q.exec()) {
        qWarning() << "listModelsByProvider failed:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QVariantMap item;

        item["id"] = q.value("id");
        item["provider_id"] = q.value("provider_id");
        item["model_key"] = q.value("model_key");
        item["display_name"] = q.value("display_name");
        item["api_key_override"] = q.value("api_key_override");
        item["context_window"] = q.value("context_window");
        item["max_output_tokens"] = q.value("max_output_tokens");
        item["enabled"] = q.value("enabled").toBool();
        item["created_at"] = q.value("created_at");
        item["updated_at"] = q.value("updated_at");

        list.append(item);
    }

    return list;
}

QVariantMap ModelRepository::getModel(const QString& id) const {
    QVariantMap item;

    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT
            id,
            provider_id,
            model_key,
            display_name,
            api_key_override,
            context_window,
            max_output_tokens,
            enabled,
            created_at,
            updated_at
        FROM models
        WHERE id = :id
        LIMIT 1
    )");

    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "getModel failed:" << q.lastError().text();
        return {};
    }

    if (!q.next())
        return {};

    item["id"] = q.value("id");
    item["provider_id"] = q.value("provider_id");
    item["model_key"] = q.value("model_key");
    item["display_name"] = q.value("display_name");
    item["api_key_override"] = q.value("api_key_override");
    item["context_window"] = q.value("context_window");
    item["max_output_tokens"] = q.value("max_output_tokens");
    item["enabled"] = q.value("enabled").toBool();
    item["created_at"] = q.value("created_at");
    item["updated_at"] = q.value("updated_at");

    return item;
}

bool ModelRepository::updateModel(
    const QString& id,
    const QString& displayName,
    const QString& apiKeyOverride,
    int contextWindow,
    int maxOutputTokens,
    bool enabled
    ) {
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE models
        SET
            display_name = :display_name,
            api_key_override = :api_key_override,
            context_window = :context_window,
            max_output_tokens = :max_output_tokens,
            enabled = :enabled,
            updated_at = :updated_at
        WHERE id = :id
    )");

    q.bindValue(":id", id);
    q.bindValue(":display_name", displayName);
    q.bindValue(":api_key_override", apiKeyOverride);
    q.bindValue(":context_window", contextWindow);
    q.bindValue(":max_output_tokens", maxOutputTokens);
    q.bindValue(":enabled", enabled ? 1 : 0);
    q.bindValue(":updated_at", QDateTime::currentSecsSinceEpoch());

    if (!q.exec()) {
        qWarning() << "updateModel failed:" << q.lastError().text();
        return false;
    }

    return true;
}

bool ModelRepository::deleteModel(const QString& id) {
    QSqlQuery q(m_db);

    q.prepare("DELETE FROM models WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "deleteModel failed:" << q.lastError().text();
        return false;
    }

    return true;
}