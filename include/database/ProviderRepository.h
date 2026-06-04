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

#include "Provider.h"

class ProviderRepository {
public:
    explicit ProviderRepository(QSqlDatabase db);

    QString createProvider(const Provider &provider);

    bool updateProvider(
        const Provider& provider);

    bool deleteProvider(const QString& id);

    Provider getProvider(const QString& id);

    QList<Provider> listProviders();

    // bool ensureProvider(const QString& id,
    //                     const QString& name,
    //                     const QString& baseUrl,
    //                     const QString& apiKey = QString());

private:
    QSqlDatabase m_db;

    qint64 now() const;
    QString generateId() const;

    bool insertProvider(const QString& id,
                        const QString& name,
                        const QString& baseUrl,
                        const QString& apiKey);
};

#endif // PROVIDERREPOSITORY_H
