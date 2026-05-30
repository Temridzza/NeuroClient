#ifndef PROVIDERSERVICE_H
#define PROVIDERSERVICE_H

#pragma once

#include <QString>
#include <QList>
#include <QVariantMap>

#include "ProviderRepository.h"
#include "SettingsService.h"

class ProviderService
{
public:
    ProviderService(
        ProviderRepository* repository,
        SettingsService* settings
        );

    // --- queries ---
    QList<QVariantMap> providers() const;

    QVariantMap currentProvider() const;
    QString currentProviderId() const;

    QString currentApiKey() const;

    QString currentBaseUrl() const;

    // --- commands ---
    void setCurrentProvider(const QString& providerId);

    QString defaultProvider() const;

private:
    ProviderRepository* m_repo;
    SettingsService* m_settings;
};

#endif // PROVIDERSERVICE_H
