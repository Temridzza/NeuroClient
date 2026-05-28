#include "AppSettings.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

/* ============================================================
 * SINGLETON
 * ============================================================ */

ChatGPTConfig& ChatGPTConfig::instance() {
    static ChatGPTConfig instance;
    return instance;
}

/* ============================================================
 * CONSTRUCTOR
 * ============================================================ */

ChatGPTConfig::ChatGPTConfig()
    : settings("NeuroClient", "NeuroClient")
{
    initDefaults();
}

/* ============================================================
 * DEFAULT VALUES
 * ============================================================ */

void ChatGPTConfig::initDefaults() {
    if (!settings.contains("apiKeyPath")) {
        settings.setValue(
            "apiKeyPath",
            "~/.config/chatgpt/api.key"
            );
    }

    if (!settings.contains("currentModel")) {
        settings.setValue("currentModel", "");
    }

    if (!settings.contains("selectedModels")) {
        settings.setValue(
            "selectedModels",
            QStringList{}
            );
    }

    if (!settings.contains("torEnabled")) {
        settings.setValue("torEnabled", false);
    }

    settings.sync();
}

/* ============================================================
 * UNIVERSAL GETTER
 * ============================================================ */

Result ChatGPTConfig::getConfigField(
    const QString &key
    ) const {
    if (!settings.contains(key)) {
        return {
            false,
            {},
            "Поле \"" + key + "\" не найдено"
        };
    }

    return {
        true,
        settings.value(key),
        ""
    };
}

/* ============================================================
 * UNIVERSAL SETTER
 * ============================================================ */

Result ChatGPTConfig::setConfigField(
    const QString &key,
    const QVariant &value
    ) {
    settings.setValue(key, value);
    settings.sync();

    return {true, {}, ""};
}

/* ============================================================
 * API KEY
 * ============================================================ */

Result ChatGPTConfig::getApiKey() {
    auto result = getApiKeyPath();

    if (!result.success) {
        return result;
    }

    QString path = result.data.toString();

    if (path.startsWith("~")) {
        path.replace(0, 1, QDir::homePath());
    }

    QFile file(path);

    if (!file.exists()) {
        return {
            false,
            {},
            "Файл API ключа не найден: " + path
        };
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {
            false,
            {},
            "Не удалось открыть файл: " + path
        };
    }

    QString key =
        QString::fromUtf8(file.readAll()).trimmed();

    file.close();

    if (key.isEmpty()) {
        return {
            false,
            {},
            "API ключ пуст"
        };
    }

    return {true, key, ""};
}

Result ChatGPTConfig::setApiKey(
    const QString &apiKey
    ) {
    auto result = getApiKeyPath();

    if (!result.success) {
        return result;
    }

    QString path = result.data.toString();

    if (path.startsWith("~")) {
        path.replace(0, 1, QDir::homePath());
    }

    QFileInfo info(path);

    QDir dir(info.absolutePath());

    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return {
                false,
                {},
                "Не удалось создать директорию"
            };
        }
    }

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return {
            false,
            {},
            "Ошибка записи API ключа"
        };
    }

    QTextStream out(&file);

    out << apiKey.trimmed();

    file.close();

    return {true, {}, ""};
}

Result ChatGPTConfig::setApiKeyPath(
    const QString &path
    ) {
    return setConfigField("apiKeyPath", path);
}

Result ChatGPTConfig::getApiKeyPath() {
    return getConfigField("apiKeyPath");
}

/* ============================================================
 * TOR
 * ============================================================ */

Result ChatGPTConfig::getTorEnable() const {
    return getConfigField("torEnabled");
}

Result ChatGPTConfig::setTorEnable(bool enabled) {
    return setConfigField(
        "torEnabled",
        enabled
        );
}

/* ============================================================
 * CURRENT MODEL
 * ============================================================ */

Result ChatGPTConfig::getCurrentModel() const {
    return getConfigField("currentModel");
}

Result ChatGPTConfig::setCurrentModel(
    const QString &model
    ) {
    return setConfigField(
        "currentModel",
        model
        );
}

/* ============================================================
 * SELECTED MODELS
 * ============================================================ */

Result ChatGPTConfig::getSelectedModels() const {
    return {
        true,
        settings.value("selectedModels").toStringList(),
        ""
    };
}

Result ChatGPTConfig::addSelectedModels(
    const QString &model
    ) {
    QStringList models =
        settings.value("selectedModels")
            .toStringList();

    if (models.contains(model)) {
        return {
            false,
            {},
            "Модель уже добавлена"
        };
    }

    models.append(model);

    return setConfigField(
        "selectedModels",
        models
        );
}

Result ChatGPTConfig::delSelectedModels(
    const QString &model
    ) {
    QStringList models =
        settings.value("selectedModels")
            .toStringList();

    if (!models.contains(model)) {
        return {
            false,
            {},
            "Модель не найдена"
        };
    }

    models.removeAll(model);

    return setConfigField(
        "selectedModels",
        models
        );
}