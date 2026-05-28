#ifndef CHATGPTCONFIG_H
#define CHATGPTCONFIG_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSettings>

struct Result {
    bool success = false;
    QVariant data;
    QString error;
};

class ChatGPTConfig {
public:
    static ChatGPTConfig& instance();

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

private:
    ChatGPTConfig();
    ~ChatGPTConfig() = default;

    QSettings settings;

    Result setConfigField(const QString &key, const QVariant &value);
    Result getConfigField(const QString &key) const;

    void initDefaults();
};

#endif // CHATGPTCONFIG_H