#pragma once

#include <QObject>
#include <QStringList>

class ModelSelectorViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QStringList models
                   READ models
                       NOTIFY modelsChanged)

    Q_PROPERTY(QString currentModel
                   READ currentModel
                       WRITE setCurrentModel
                           NOTIFY currentModelChanged)

public:
    explicit ModelSelectorViewModel(QObject *parent = nullptr);

    QStringList models() const;

    QString currentModel() const;
    void setCurrentModel(const QString &model);

signals:
    void modelsChanged();
    void currentModelChanged();

private:
    QStringList m_models;
    QString m_currentModel;

    void loadModels();
};