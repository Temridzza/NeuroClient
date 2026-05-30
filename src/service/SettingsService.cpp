#include "SettingsService.h"

#include <QDebug>

SettingsService::SettingsService(
    AppSettings* settings,
    QObject* parent
    )
    : QObject(parent)
    , m_settings(settings)
{
}

QString SettingsService::currentProviderId() const
{
    return m_settings->currentProviderId();
}

QString SettingsService::currentModelId() const
{
    return m_settings->currentModelId();
}

Result SettingsService::setCurrentProviderId(const QString& id)
{
    if (id.isEmpty()) {
        qWarning() << "SettingsService:"
                   << "Provider id is empty";

        return {
            false,
            {},
            "Provider id is empty"
        };
    }

    m_settings->setCurrentProviderId(id);

    emit currentProviderIdChanged(id);

    return {
        true,
        {},
        {}
    };
}

Result SettingsService::setCurrentModelId(const QString& id)
{
    if (id.isEmpty()) {
        qWarning() << "SettingsService:"
                   << "Model id is empty";

        return {
            false,
            {},
            "Model id is empty"
        };
    }

    m_settings->setCurrentModelId(id);

    emit currentModelIdChanged(id);

    return {
        true,
        {},
        {}
    };
}

QString SettingsService::currentChatId() const
{
    return m_settings->currentChatId();
}

Result SettingsService::setCurrentChatId(const QString& id)
{
    if (id.isEmpty()) {
        qWarning() << "SettingsService:" << "Chat id is empty";

        return { false, {}, "Chat id is empty" };
    }

    m_settings->setCurrentChatId(id);

    emit currentChatIdChanged(id);

    return { true, {}, {} };
}