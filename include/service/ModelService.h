#ifndef MODELSERVICE_H
#define MODELSERVICE_H

#pragma once

#include <QString>
#include <QList>

#include "ModelRepository.h"
#include "SettingsService.h"
#include "Model.h"

class ModelService
{
public:
    ModelService(
        ModelRepository* repository,
        SettingsService* settings
        );

    // --- queries ---
    QList<Model> models() const;
    QList<Model> modelsByProvider(const QString& providerId) const;

    Model currentModel(const QString modelId) const;
    QString currentModelId() const;
    QString currentModelKey(const QString modelId) const;

    // --- commands ---
    void setCurrentModel(const QString& modelId);

    QString defaultModelForProvider(const QString& providerId) const;
    bool updateModel(const Model& model);
    QString createModel(const Model &model);
    bool deleteModel(const QString& id);

private:
    ModelRepository* m_repo;
    SettingsService* m_settings;
};

#endif // MODELSERVICE_H
