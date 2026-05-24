#ifndef CHATGPTCONFIG_H
#define CHATGPTCONFIG_H

#include <QString>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct Result {
    bool success = false;
    QVariant data;      // Может быть QString, bool, QStringList, QJsonObject, QJsonArray...
    QString error;      // Сообщение об ошибке
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

    // Models
    Result setCurrentModel(const QString &str);
    Result getCurrentModel() const;

    // SelectedModel
    Result getSelectedModels();
    Result addSelectedModels(const QString &model);
    Result delSelectedModels(const QString &model);

private:
    ChatGPTConfig();
    ~ChatGPTConfig() = default;

    QString directoryPath = "../userConfig";
    QJsonDocument Config;

    // Universal getter/setter
    Result setConfigField(const QString &key, const QVariant &value);
    Result getConfigField(const QString &key) const;

    // Main operations
    Result loadConfig();
    Result saveConfig();
};

#endif // CHATGPTCONFIG_H
