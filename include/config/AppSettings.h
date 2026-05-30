#ifndef AppSettings_H
#define AppSettings_H

#include <QSettings>
#include <QString>

class AppSettings : public QObject
{
public:
    explicit AppSettings();
    ~AppSettings() = default;

    QString currentProviderId() const;
    void setCurrentProviderId(const QString& id);

    QString currentModelId() const;
    void setCurrentModelId(const QString& id);

private:
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(const AppSettings&) = delete;

    void initDefaults();

private:
    QSettings m_settings;
};

#endif // AppSettings_H