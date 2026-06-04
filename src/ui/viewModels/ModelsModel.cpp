#include "ModelsModel.h"

ModelsModel::ModelsModel(ModelService *service, QObject* parent)
    : QObject(parent),
    m_service(service)
{
}

static QVariantMap toVariant(const Model& model)
{
    return {
        {"id", model.id},
        {"providerId", model.providerId},
        {"modelKey", model.modelKey},
        {"display_name", model.displayName},
        {"apiKeyOverride", model.apiKeyOverride},
        {"prompt", model.prompt},
        {"contextWindow", model.contextWindow},
        {"maxOutputTokens", model.maxOutputTokens},
        {"enabled", model.enabled},
        {"createdAt", model.createdAt},
        {"updatedAt", model.updatedAt}
    };
}

QVariantList ModelsModel::models() const {
    return m_models;
}

void ModelsModel::loadModels() {
    setModels(m_service->models());
}

void ModelsModel::loadModelsByProvider(const QString& providerId) {
    setModels(m_service->modelsByProvider(providerId));
}

QString ModelsModel::createModel(
    const QString& providerId,
    const QString& modelKey,
    const QString& displayName,
    const QString& apiKeyOverride,
    int contextWindow,
    int maxOutputTokens,
    bool enabled
    ) {

    Model model;
    model.providerId = providerId;
    model.modelKey = modelKey;
    model.displayName = displayName;
    model.contextWindow = contextWindow;
    model.maxOutputTokens = maxOutputTokens;
    model.enabled = enabled;

    const QString id = m_service->createModel(model);

    if (!id.isEmpty()) {
        loadModelsByProvider(providerId);
    }

    return id;
}

bool ModelsModel::updateModel(
    const QString& id,
    const QString& displayName,
    const QString& apiKeyOverride,
    int contextWindow,
    int maxOutputTokens,
    bool enabled
    ) {
    Model model;
    model.id = id;
    model.displayName = displayName;
    model.apiKeyOverride = apiKeyOverride;
    model.contextWindow = contextWindow;
    model.maxOutputTokens = maxOutputTokens;
    model.enabled = enabled;

    const bool ok = m_service->updateModel(model);

    if (ok) {
        loadModels();
    }

    return ok;
}

bool ModelsModel::deleteModel(const QString& id) {
    const bool ok = m_service->deleteModel(id);
    if (ok) {
        loadModels();
    }

    return ok;
}

void ModelsModel::setModels(const QList<Model>& list)
{
    m_models.clear();

    for (const auto& model : list)
    {
        m_models.append(toVariant(model));
    }

    emit modelsChanged();
}