#include "ModelSelector.h"

ModelSelector::ModelSelector(
    SettingsService *m_settings,
    QObject *parent
)
    : QObject(parent),
    m_settings(m_settings)
{
    loadModels();

    m_currentModel = m_settings->currentModelId();
}

QStringList ModelSelector::models() const {
    return m_models;
}

QString ModelSelector::currentModel() const {
    return m_currentModel;
}

void ModelSelector::setCurrentModel(const QString &model) {
    if (m_currentModel == model)
        return;

    m_currentModel = model;

    m_settings->setCurrentModelId(model);

    emit currentModelChanged();
}

void ModelSelector::loadModels() {
    // Result result = AppSettings::instance().selectedModelId();

    // if (!result.success)
    //     return;

    // QStringList list;

    // for (const auto &v : result.data.toJsonArray())
    //     list << v.toString();

    // m_models = list;

    emit modelsChanged();
}