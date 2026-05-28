#ifndef ModelsModel_H
#define ModelsModel_H

#pragma once

#include <QObject>
#include <QVariantList>

#include "ModelRepository.h"

class ModelsModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariantList models
                   READ models
                       NOTIFY modelsChanged)

public:
    explicit ModelsModel(QSqlDatabase db, QObject* parent = nullptr);

    QVariantList models() const;

    Q_INVOKABLE void loadModels();

    Q_INVOKABLE void loadModelsByProvider(const QString& providerId);

    Q_INVOKABLE QString createModel(
        const QString& providerId,
        const QString& modelKey,
        const QString& displayName,
        const QString& apiKeyOverride = QString(),
        int contextWindow = 0,
        int maxOutputTokens = 0,
        bool enabled = true
        );

    Q_INVOKABLE bool updateModel(
        const QString& id,
        const QString& displayName,
        const QString& apiKeyOverride,
        int contextWindow,
        int maxOutputTokens,
        bool enabled
        );

    Q_INVOKABLE bool deleteModel(const QString& id);

signals:
    void modelsChanged();

private:
    void setModels(const QList<QVariantMap>& list);

private:
    ModelRepository m_repo;
    QVariantList m_models;
};

#endif // ModelsModel_H
