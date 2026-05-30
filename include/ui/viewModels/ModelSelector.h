#pragma once

#include <QObject>
#include <QStringList>
#include "SettingsService.h"
#include "QDebug"
#include "QJsonArray"
#include "QString"

class ModelSelector : public QObject {
    Q_OBJECT

    Q_PROPERTY(QStringList models
                   READ models
                       NOTIFY modelsChanged)

    Q_PROPERTY(QString currentModel
                   READ currentModel
                       WRITE setCurrentModel
                           NOTIFY currentModelChanged)

public:
    explicit ModelSelector(
        SettingsService* m_settings,
        QObject *parent = nullptr
        );

    QStringList models() const;

    QString currentModel() const;
    void setCurrentModel(const QString &model);

signals:
    void modelsChanged();
    void currentModelChanged();

private:
    QStringList m_models;
    QString m_currentModel;
    SettingsService* m_settings;

    void loadModels();
};