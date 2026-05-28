#ifndef MODELREPOSITORY_H
#define MODELREPOSITORY_H

#pragma once

#include <QString>
#include <QList>
#include <QVariantMap>
#include <QDateTime>
#include <QUuid>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

class ModelRepository {
public:
    explicit ModelRepository(QSqlDatabase db);

    QString createModel(
        const QString& providerId,
        const QString& modelKey,
        const QString& displayName,
        const QString& apiKeyOverride = QString(),
        int contextWindow = 0,
        int maxOutputTokens = 0,
        bool enabled = true
        );

    QList<QVariantMap> listModels() const;
    QList<QVariantMap> listModelsByProvider(const QString& providerId) const;

    QVariantMap getModel(const QString& id) const;

    bool updateModel(
        const QString& id,
        const QString& displayName,
        const QString& apiKeyOverride,
        int contextWindow,
        int maxOutputTokens,
        bool enabled
        );

    bool deleteModel(const QString& id);

private:
    QSqlDatabase m_db;
};

#endif // MODELREPOSITORY_H
