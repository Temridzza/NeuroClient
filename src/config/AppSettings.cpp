#include "AppSettings.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

/* ============================================================
 * SINGLETON
 * ============================================================ */

AppSettings& AppSettings::instance() {
    static AppSettings instance;
    return instance;
}

/* ============================================================
 * CONSTRUCTOR
 * ============================================================ */

AppSettings::AppSettings(QObject *parent)
    : QObject(nullptr),
    settings("NeuroClient", "NeuroClient")
{
    initDefaults();
}

/* ============================================================
 * DEFAULT VALUES
 * ============================================================ */

void AppSettings::initDefaults() {
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

Result AppSettings::getConfigField(
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

Result AppSettings::setConfigField(
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

Result AppSettings::getApiKey() {
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

Result AppSettings::setApiKey(
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

Result AppSettings::setApiKeyPath(
    const QString &path
    ) {
    return setConfigField("apiKeyPath", path);
}

Result AppSettings::getApiKeyPath() {
    return getConfigField("apiKeyPath");
}

/* ============================================================
 * TOR
 * ============================================================ */

Result AppSettings::getTorEnable() const {
    return getConfigField("torEnabled");
}

Result AppSettings::setTorEnable(bool enabled) {
    return setConfigField(
        "torEnabled",
        enabled
        );
}

/* ============================================================
 * CURRENT MODEL
 * ============================================================ */

Result AppSettings::getCurrentModel() const {
    return getConfigField("currentModel");
}

Result AppSettings::setCurrentModel(
    const QString &model
    ) {

    auto result =
        setConfigField(
            "currentModel",
            model
            );

    if (result.success) {
        emit currentModelChanged(model);
    }

    return result;
}

/* ============================================================
 * SELECTED MODELS
 * ============================================================ */

Result AppSettings::getSelectedModels() const {
    return {
        true,
        settings.value("selectedModels").toStringList(),
        ""
    };
}

Result AppSettings::addSelectedModels(
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

Result AppSettings::delSelectedModels(
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