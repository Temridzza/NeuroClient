#ifndef MODELREPOSITORY_H
#define MODELREPOSITORY_H

#pragma once

#include <QString>
#include <QList>
#include <QDateTime>
#include <QUuid>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

#include "Model.h"

class ModelRepository {
public:
    explicit ModelRepository(QSqlDatabase db);

    QString createModel(
        const Model& model
        );

    QList<Model> listModels() const;
    QList<Model> listModelsByProvider(const QString& providerId) const;

    Model getModel(const QString& id) const;

    bool updateModel(
        const Model& model
        );

    bool deleteModel(const QString& id);

    bool ensureModel(const QString& id,
                     const QString& providerId,
                     const QString& modelKey,
                     const QString& displayName,
                     bool enabled = true);

private:
    QSqlDatabase m_db;
    static Model readModel(
        const QSqlQuery& q
        );
};

#endif // MODELREPOSITORY_H
