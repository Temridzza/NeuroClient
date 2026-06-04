#ifndef MODEL_H
#define MODEL_H

#pragma once

#include <QString>

struct Model
{
    QString id;

    QString providerId;

    QString modelKey;
    QString displayName;

    QString apiKeyOverride;

    QString prompt;

    int contextWindow = 0;
    int maxOutputTokens = 0;

    bool enabled = true;

    qint64 createdAt = 0;
    qint64 updatedAt = 0;

    bool isValid() const
    {
        return !id.isEmpty();
    }
};

#endif // MODEL_H
