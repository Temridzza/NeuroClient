#ifndef MODELSERVICE_H
#define MODELSERVICE_H

#pragma once

#include <QString>
#include <QList>
#include <QVariantMap>

#include "ModelRepository.h"
#include "SettingsService.h"

class ModelService
{
public:
    ModelService(
        ModelRepository* repository,
        SettingsService* settings
        );

    // --- queries ---
    QList<QVariantMap> models() const;
    QList<QVariantMap> modelsByProvider(const QString& providerId) const;

    QVariantMap currentModel() const;
    QString currentModelId() const;
    QString currentModelKey() const;

    // --- commands ---
    void setCurrentModel(const QString& modelId);

    QString defaultModelForProvider(const QString& providerId) const;

private:
    ModelRepository* m_repo;
    SettingsService* m_settings;
};

#endif // MODELSERVICE_H
