#include "ModelRepository.h"

ModelRepository::ModelRepository(QSqlDatabase db)
    : m_db(db) {}

QString ModelRepository::createModel(
    const Model& model
    )
{
    const QString id =
        QUuid::createUuid()
            .toString(QUuid::WithoutBraces);

    const qint64 now =
        QDateTime::currentSecsSinceEpoch();

    QSqlQuery q(m_db);

    q.prepare(R"(
        INSERT INTO models (
            id,
            provider_id,
            model_key,
            display_name,
            api_key_override,
            prompt,
            context_window,
            max_output_tokens,
            enabled,
            created_at,
            updated_at
        )
        VALUES (
            ?, ?, ?, ?, ?,
            ?, ?, ?, ?, ?, ?
        )
    )");

    q.addBindValue(id);
    q.addBindValue(model.providerId);
    q.addBindValue(model.modelKey);
    q.addBindValue(model.displayName);
    q.addBindValue(model.apiKeyOverride);

    q.addBindValue(model.prompt);

    q.addBindValue(model.contextWindow);
    q.addBindValue(model.maxOutputTokens);
    q.addBindValue(model.enabled ? 1 : 0);

    q.addBindValue(now);
    q.addBindValue(now);

    if (!q.exec()) {
        qWarning()
        << "createModel failed:"
        << q.lastError().text();

        return {};
    }

    return id;
}

QList<Model>
ModelRepository::listModels() const
{
    QList<Model> models;

    QSqlQuery q(m_db);

    if (!q.exec(R"(
        SELECT *
        FROM models
        ORDER BY display_name
    )"))
    {
        qWarning()
        << q.lastError().text();

        return models;
    }

    while (q.next())
        models.append(readModel(q));

    return models;
}

QList<Model>
ModelRepository::listModelsByProvider(
    const QString& providerId
    ) const
{
    QList<Model> models;

    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT *
        FROM models
        WHERE provider_id = ?
        ORDER BY display_name
    )");

    q.addBindValue(providerId);

    if (!q.exec()) {
        qWarning()
        << "listModelsByProvider failed:"
        << q.lastError().text();

        return models;
    }

    while (q.next()) {
        models.append(readModel(q));
    }

    return models;
}

Model ModelRepository::getModel(
    const QString& id
    ) const
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT *
        FROM models
        WHERE id = ?
        LIMIT 1
    )");

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning()
        << "getModel failed:"
        << q.lastError().text();

        return {};
    }

    if (!q.next())
        return {};

    return readModel(q);
}

bool ModelRepository::updateModel(
    const Model& model
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        UPDATE models
        SET
            display_name = ?,
            api_key_override = ?,
            prompt = ?,
            context_window = ?,
            max_output_tokens = ?,
            enabled = ?,
            updated_at = ?
        WHERE id = ?
    )");

    q.addBindValue(model.displayName);
    q.addBindValue(model.apiKeyOverride);

    q.addBindValue(model.prompt);

    q.addBindValue(model.contextWindow);
    q.addBindValue(model.maxOutputTokens);

    q.addBindValue(model.enabled ? 1 : 0);

    q.addBindValue(
        QDateTime::currentSecsSinceEpoch()
        );

    q.addBindValue(model.id);

    if (!q.exec()) {
        qWarning()
        << "updateModel failed:"
        << q.lastError().text();

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool ModelRepository::deleteModel(
    const QString& id
    )
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        DELETE FROM models
        WHERE id = ?
    )");

    q.addBindValue(id);

    if (!q.exec()) {
        qWarning()
        << "deleteModel failed:"
        << q.lastError().text();

        return false;
    }

    return q.numRowsAffected() > 0;
}

Model ModelRepository::readModel(
    const QSqlQuery& q
    )
{
    Model model;

    model.id =
        q.value("id").toString();

    model.providerId =
        q.value("provider_id").toString();

    model.modelKey =
        q.value("model_key").toString();

    model.displayName =
        q.value("display_name").toString();

    model.apiKeyOverride =
        q.value("api_key_override").toString();

    model.prompt =
        q.value("prompt").toString();

    model.contextWindow =
        q.value("context_window").toInt();

    model.maxOutputTokens =
        q.value("max_output_tokens").toInt();

    model.enabled =
        q.value("enabled").toBool();

    model.createdAt =
        q.value("created_at").toLongLong();

    model.updatedAt =
        q.value("updated_at").toLongLong();

    return model;
}