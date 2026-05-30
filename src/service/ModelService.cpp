#include "ModelService.h"

ModelService::ModelService(
    ModelRepository* repository,
    SettingsService* settings
    )
    : m_repo(repository),
    m_settings(settings)
{
}

QList<QVariantMap> ModelService::models() const
{
    return m_repo->listModels();
}

QList<QVariantMap> ModelService::modelsByProvider(const QString& providerId) const
{
    return m_repo->listModelsByProvider(providerId);
}

QString ModelService::currentModelId() const
{
    return m_settings->currentModelId();
}

QVariantMap ModelService::currentModel() const
{
    const QString id = m_settings->currentModelId();
    if (id.isEmpty())
        return {};

    return m_repo->getModel(id);
}

void ModelService::setCurrentModel(const QString& modelId)
{
    if (modelId.isEmpty())
        return;

    m_settings->setCurrentModelId(modelId);
}

QString ModelService::defaultModelForProvider(const QString& providerId) const
{
    const auto models = m_repo->listModelsByProvider(providerId);

    if (models.isEmpty())
        return {};

    return models.first().value("id").toString();
}

QString ModelService::currentModelKey() const
{
    const auto model = currentModel();

    return model.value("model_key").toString();
}