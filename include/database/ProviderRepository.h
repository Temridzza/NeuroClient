#ifndef PROVIDERREPOSITORY_H
#define PROVIDERREPOSITORY_H

#pragma once

#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QUuid>
#include <QDebug>

class ProviderRepository {
public:
    explicit ProviderRepository(QSqlDatabase db);

    QString createProvider(const QString& name,
                           const QString& baseUrl,
                           const QString& apiKey = QString());

    bool updateProvider(const QString& id,
                        const QVariantMap& fields);

    bool deleteProvider(const QString& id);

    QVariantMap getProvider(const QString& id);

    QList<QVariantMap> listProviders();

private:
    QSqlDatabase m_db;

    qint64 now() const;
    QString generateId() const;
};

#endif // PROVIDERREPOSITORY_H
