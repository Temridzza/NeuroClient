#ifndef AppSettings_H
#define AppSettings_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSettings>
#include <QObject>

struct Result {
    bool success = false;
    QVariant data;
    QString error;
};

class AppSettings : public QObject  {
    Q_OBJECT
public:
    static AppSettings& instance();

    // API Key
    Result getApiKey();
    Result setApiKey(const QString &apiKey);

    Result setApiKeyPath(const QString &path);
    Result getApiKeyPath();

    // Tor
    Result getTorEnable() const;
    Result setTorEnable(bool enabled);

    // Current model
    Result getCurrentModel() const;
    Result setCurrentModel(const QString &model);

    // Selected models
    Result getSelectedModels() const;
    Result addSelectedModels(const QString &model);
    Result delSelectedModels(const QString &model);

signals:
    void currentModelChanged(
        const QString& model
        );

private:
    explicit AppSettings(
        QObject* parent = nullptr
        );

    ~AppSettings() = default;

    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(
        const AppSettings&
        ) = delete;

    QSettings settings;

    Result setConfigField(const QString &key, const QVariant &value);
    Result getConfigField(const QString &key) const;

    void initDefaults();

};

#endif // AppSettings_H