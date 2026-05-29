#ifndef ProviderModel_H
#define ProviderModel_H

#pragma once

#include <QAbstractListModel>
#include <QVariantMap>
#include <QSqlDatabase>

#include "ProviderRepository.h"

class ProviderModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        BaseUrlRole,
        ApiKeyRole,
        CreatedAtRole,
        UpdatedAtRole
    };

    explicit ProviderModel(
        ProviderRepository* repo,
        QObject* parent = nullptr
        );

    // QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // API для QML
    Q_INVOKABLE void loadProviders();
    Q_INVOKABLE QString createProvider(const QString& name,
                                       const QString& baseUrl,
                                       const QString& apiKey = "");

    Q_INVOKABLE bool updateProvider(const QString& id,
                                    const QVariantMap& fields);

    Q_INVOKABLE bool deleteProvider(const QString& id);
    Q_INVOKABLE QVariantMap getProviderById(const QString& id) const;

    Q_INVOKABLE QVariantMap getProvider(int row) const;

private:
    ProviderRepository m_repo;
    QList<QVariantMap> m_items;

    void reloadFromDb();
};

#endif // ProviderModel_H
