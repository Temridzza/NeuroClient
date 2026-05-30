#ifndef PROVIDERSELECTOR_H
#define PROVIDERSELECTOR_H

#pragma once

#include <QObject>
#include <QVariantList>
#include <QDebug>

#include "ProviderService.h"

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
    explicit ProviderSelector(
        ProviderService* service,
        QObject *parent = nullptr
        );

    QVariantList providers() const;
    QString currentProvider() const;

    void setCurrentProvider(const QString &provider);
    QString currentProviderDisplayName() const;

    int currentProviderIndex() const;

signals:
    void providersChanged();
    void currentProviderChanged();

private:
    QVariantList m_providers;
    QString m_currentProvider;

    ProviderService* m_service;

    void reload();
};

#endif // PROVIDERSELECTOR_H
