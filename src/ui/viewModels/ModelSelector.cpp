#include "ModelSelector.h"
#include "AppSettings.h"
#include "QDebug"
#include "QJsonArray"

ModelSelector::ModelSelector(QObject *parent)
    : QObject(parent)
{
    loadModels();

    Result current = AppSettings::instance().getCurrentModel();

    if (current.success) {
        m_currentModel = current.data.toString();
    }
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

    AppSettings::instance().setCurrentModel(model);

    emit currentModelChanged();
}

void ModelSelector::loadModels() {
    Result result = AppSettings::instance().getSelectedModels();

    if (!result.success)
        return;

    QStringList list;

    for (const auto &v : result.data.toJsonArray())
        list << v.toString();

    m_models = list;

    emit modelsChanged();
}