#include "ModelSelector.h"

ModelSelector::ModelSelector(
    QObject *parent
    )
    : QObject(parent)
{
}

QVariantList ModelSelector::models()
{
    return m_models;
}

QString ModelSelector::currentModel() const
{
    return m_currentModel;
}

void ModelSelector::setCurrentModel(const QString &model)
{
    if (m_currentModel == model)
        return;

    m_currentModel = model;

    emit currentModelChanged();
}

void ModelSelector::setState(const QVariantList& models, const QString& current)
{
    m_models = models;
    emit modelsChanged();

    bool found = false;
    for (const auto& m : m_models) {
        if (m.toMap()["id"].toString() == current) {
            found = true;
            break;
        }
    }

    m_currentModel = found && !current.isEmpty()
                         ? current
                         : (m_models.isEmpty()
                                ? QString()
                                : m_models.first().toMap()["id"].toString());

    emit currentModelChanged();
}

int ModelSelector::currentModelIndex() const
{
    for (int i = 0; i < m_models.size(); ++i) {
        const auto map = m_models[i].toMap();
        if (map.value("id").toString() == m_currentModel)
            return i;
    }

    return 0;
}