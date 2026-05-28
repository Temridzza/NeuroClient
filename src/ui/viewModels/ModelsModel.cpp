#include "ModelsModel.h"

ModelsModel::ModelsModel(QSqlDatabase db, QObject* parent)
    : QObject(parent),
    m_repo(db) {}

QVariantList ModelsModel::models() const {
    return m_models;
}

void ModelsModel::loadModels() {
    setModels(m_repo.listModels());
}

void ModelsModel::loadModelsByProvider(const QString& providerId) {
    setModels(m_repo.listModelsByProvider(providerId));
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
    const QString id = m_repo.createModel(
        providerId,
        modelKey,
        displayName,
        apiKeyOverride,
        contextWindow,
        maxOutputTokens,
        enabled
        );

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
    const bool ok = m_repo.updateModel(
        id,
        displayName,
        apiKeyOverride,
        contextWindow,
        maxOutputTokens,
        enabled
        );

    if (ok) {
        loadModels();
    }

    return ok;
}

bool ModelsModel::deleteModel(const QString& id) {
    const bool ok = m_repo.deleteModel(id);

    if (ok) {
        loadModels();
    }

    return ok;
}

void ModelsModel::setModels(const QList<QVariantMap>& list) {
    m_models.clear();

    for (const auto& item : list) {
        m_models.append(item);
    }

    emit modelsChanged();
}