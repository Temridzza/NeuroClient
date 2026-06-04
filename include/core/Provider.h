#ifndef PROVIDER_H
#define PROVIDER_H

#pragma once

#include <QString>

struct Provider
{
    QString id;

    QString name;
    QString baseUrl;
    QString apiKey;

    QString prompt;

    qint64 createdAt = 0;
    qint64 updatedAt = 0;

    bool isValid() const
    {
        return !id.isEmpty();
    }
};

#endif // PROVIDER_H
