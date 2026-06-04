#include "ModelService.h"

ModelService::ModelService(
    ModelRepository* repository,
    SettingsService* settings
    )
    : m_repo(repository),
    m_settings(settings)
{
}

QList<Model> ModelService::models() const
{
    return m_repo->listModels();
}

QList<Model> ModelService::modelsByProvider(const QString& providerId) const
{
    return m_repo->listModelsByProvider(providerId);
}

QString ModelService::currentModelId() const
{
    return m_settings->currentModelId();
}

Model ModelService::currentModel(const QString modelId) const
{
    return m_repo->getModel(modelId);
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

    return models.first().id;
}

QString ModelService::currentModelKey(const QString modelId) const
{
    const auto model = currentModel(modelId);

    return model.modelKey;
}

bool ModelService::updateModel(const Model& model)
{
    return m_repo->updateModel(model);
}
QString ModelService::createModel(const Model& model)
{
    return m_repo->createModel(model);
}

bool ModelService::deleteModel(const QString &id)
{
    return m_repo->deleteModel(id);
}