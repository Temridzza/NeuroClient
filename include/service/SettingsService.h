#ifndef SETTINGSSERVICE_H
#define SETTINGSSERVICE_H

#include <QObject>

#include "AppSettings.h"
#include "Result.h"
#include "service/SettingsService.h"

class SettingsService : public QObject
{
    Q_OBJECT

public:
    explicit SettingsService(
        AppSettings* settings,
        QObject* parent = nullptr
        );

    QString currentProviderId() const;
    QString currentModelId() const;

    Result setCurrentProviderId(const QString& id);
    Result setCurrentModelId(const QString& id);

    QString currentChatId() const;
    Result setCurrentChatId(const QString& id);

signals:
    void currentProviderIdChanged(const QString& id);
    void currentModelIdChanged(const QString& id);

    void currentChatIdChanged(const QString& id);

private:
    AppSettings* m_settings {};
};

#endif // SETTINGSSERVICE_H
