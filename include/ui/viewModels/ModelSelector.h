#pragma once

#include <QObject>
#include <QStringList>

#include "ModelService.h"

class ModelSelector : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariantList models
                   READ models
                       NOTIFY modelsChanged)

    Q_PROPERTY(QString currentModel
                   READ currentModel
                       WRITE setCurrentModel
                           NOTIFY currentModelChanged)
    Q_PROPERTY(int currentModelIndex
                   READ currentModelIndex
                       NOTIFY currentModelChanged)

public:
    explicit ModelSelector(
        QObject *parent = nullptr
        );

    QVariantList models();
    QString currentModel() const;

    void setCurrentModel(const QString &model);
    void setState(const QVariantList& models,
                  const QString& current);
    int currentModelIndex() const;


signals:
    void modelsChanged();
    void currentModelChanged();

private:
    QVariantList m_models;
    QString m_currentModel;

    ModelService* m_service;

    QString m_providerId;
};