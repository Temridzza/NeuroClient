#include "ModelSelector.h"

ModelSelector::ModelSelector(
    QObject *parent
    )
    : QObject(parent)
{
}

QVariantList ModelSelector::models()
{
    QVariantList list;
    for (const auto &m : m_modelList) {
        QVariantMap map;
        map["id"] = m.id;
        map["providerId"] = m.providerId;
        map["modelKey"] = m.modelKey;
        map["display_name"] = m.displayName;
        map["apiKeyOverride"] = m.apiKeyOverride;
        map["prompt"] = m.prompt;
        map["contextWindow"] = m.contextWindow;
        map["maxOutputTokens"] = m.maxOutputTokens;
        map["enabled"] = m.enabled;
        map["createdAt"] = m.createdAt;
        map["updatedAt"] = m.updatedAt;
        list.append(map);
    }
    return list;
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

    emit currentModelChanged(model);
}

void ModelSelector::setState(const QList<Model> &models, const QString &current)
{
    m_modelList = models;

    bool found = false;
    for (const auto &m : m_modelList) {
        if (m.id == current) {
            found = true;
            break;
        }
    }

    m_currentModel =
        (found && !current.isEmpty())
            ? current
            : (m_modelList.isEmpty() ? QString() : m_modelList.first().id);

    emit modelsChanged();
    emit currentModelChanged(m_currentModel);
}

int ModelSelector::currentModelIndex() const
{
    for (int i = 0; i < m_modelList.size(); ++i) {
        if (m_modelList[i].id == m_currentModel)
            return i;
    }

    return 0;
}