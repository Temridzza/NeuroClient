#ifndef PROVIDERSELECTOR_H
#define PROVIDERSELECTOR_H

#pragma once

#include <QObject>
#include <QVariantList>
#include "ProviderService.h"
#include "Provider.h"

class ProviderSelector : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariantList providers
                   READ providers
                       NOTIFY providersChanged)

    Q_PROPERTY(QString currentProvider
                   READ currentProvider
                       WRITE setCurrentProvider
                           NOTIFY currentProviderChanged)

    Q_PROPERTY(int currentProviderIndex
                   READ currentProviderIndex
                       NOTIFY currentProviderChanged)

public:
    explicit ProviderSelector(ProviderService* service,
                              QObject *parent = nullptr);

    QVariantList providers() const;
    QString currentProvider() const;

    void setCurrentProvider(const QString &provider);
    QString currentProviderDisplayName() const;
    int currentProviderIndex() const;

signals:
    void providersChanged();
    void currentProviderChanged(const QString &provider);

private:
    QList<Provider> m_providerList;  // внутреннее хранение
    QString m_currentProvider;

    ProviderService* m_service = nullptr;

    void reload();
};

#endif // PROVIDERSELECTOR_H
