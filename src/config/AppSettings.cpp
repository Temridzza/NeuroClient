#include "AppSettings.h"

/* ============================================================
 * CONSTRUCTOR
 * ============================================================ */

AppSettings::AppSettings():
    m_settings("NeuroClient", "NeuroClient")
{
    initDefaults();
}

/* ============================================================
 * DEFAULT VALUES
 * ============================================================ */

void AppSettings::initDefaults()
{
    if (!m_settings.contains("session/currentProviderId")) {
        m_settings.setValue(
            "session/currentProviderId",
            -1
            );
    }

    if (!m_settings.contains("session/currentModelId")) {
        m_settings.setValue(
            "session/currentModelId",
            -1
            );
    }

    m_settings.sync();
}

/* ============================================================
 * CURRENT PROVIDER
 * ============================================================ */

QString AppSettings::currentProviderId() const
{
    return m_settings.value(
                         "session/currentProviderId",
                         -1
                         ).toString();
}

void AppSettings::setCurrentProviderId(const QString &id)
{
    if (currentProviderId() == id) {
        return;
    }

    m_settings.setValue(
        "session/currentProviderId",
        id
        );

    m_settings.sync();
}

/* ============================================================
 * CURRENT MODEL
 * ============================================================ */

QString AppSettings::currentModelId() const
{
    return m_settings.value(
                         "session/currentModelId",
                         -1
                         ).toString();
}

void AppSettings::setCurrentModelId(const QString &id)
{
    if (currentModelId() == id) {
        return;
    }

    m_settings.setValue(
        "session/currentModelId",
        id
        );

    m_settings.sync();
}