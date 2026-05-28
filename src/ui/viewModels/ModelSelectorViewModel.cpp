#include "ModelSelectorViewModel.h"
#include "AppSettings.h"
#include "QDebug"
#include "QJsonArray"

ModelSelectorViewModel::ModelSelectorViewModel(QObject *parent)
    : QObject(parent)
{
    loadModels();

    Result current = AppSettings::instance().getCurrentModel();

    if (current.success) {
        m_currentModel = current.data.toString();
    }
}

QStringList ModelSelectorViewModel::models() const {
    return m_models;
}

QString ModelSelectorViewModel::currentModel() const {
    return m_currentModel;
}

void ModelSelectorViewModel::setCurrentModel(const QString &model) {
    if (m_currentModel == model)
        return;

    m_currentModel = model;

    AppSettings::instance().setCurrentModel(model);

    emit currentModelChanged();
}

void ModelSelectorViewModel::loadModels() {
    Result result = AppSettings::instance().getSelectedModels();

    if (!result.success)
        return;

    QStringList list;

    for (const auto &v : result.data.toJsonArray())
        list << v.toString();

    m_models = list;

    emit modelsChanged();
}