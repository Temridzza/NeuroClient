#ifndef PROVIDERSERVICE_H
#define PROVIDERSERVICE_H

#pragma once

#include <QString>
#include <QList>

#include "ProviderRepository.h"
#include "SettingsService.h"
#include "Provider.h"

class ProviderService
{
public:
    ProviderService(
        ProviderRepository* repository,
        SettingsService* settings
        );

    // --- queries ---
    QList<Provider> providers() const;

    Provider currentProvider(const QString &providerId) const;
    QString currentProviderId() const;

    QString currentApiKey(const QString &providerId) const;

    QString currentBaseUrl(const QString &providerId) const;

    // --- commands ---
    void setCurrentProvider(const QString &providerId);

    QString defaultProvider() const;

    bool updateProvider(const Provider& provider);

    bool deleteProvider(
        const QString& id
        );

    QString createProvider(const Provider& provider);
private:
    ProviderRepository* m_repo;
    SettingsService* m_settings;
};

#endif // PROVIDERSERVICE_H
