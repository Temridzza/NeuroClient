#include "ChatGPTConfig.h"

#include <filesystem>
#include <iostream>

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>


// Singleton instance
ChatGPTConfig& ChatGPTConfig::instance() {
    static ChatGPTConfig instance;
    return instance;
}

ChatGPTConfig::ChatGPTConfig() {
    this->loadConfig();
}

/* ============================================================
 * UNIVERSAL CONFIG FIELD GETTER
 * ============================================================ */

Result ChatGPTConfig::getConfigField(const QString &key) const {
    if (this->Config.isNull() || !this->Config.isObject())
        return {false, {}, "Конфигурация не инициализирована"};

    QJsonObject obj = this->Config.object();

    if (!obj.contains(key))
        return {false, {}, "Поле \"" + key + "\" не найдено"};

    QJsonValue val = obj[key];

    // Автоматически определяем тип
    if (val.isString())
        return {true, val.toString(), ""};

    if (val.isBool())
        return {true, val.toBool(), ""};

    if (val.isDouble())
        return {true, val.toDouble(), ""};

    if (val.isArray())
        return {true, val.toArray(), ""};

    if (val.isObject())
        return {true, val.toObject(), ""};

    return {false, {}, "Неизвестный тип поля: " + key};
}

/* ============================================================
 * UNIVERSAL CONFIG FIELD SETTER
 * ============================================================ */

Result ChatGPTConfig::setConfigField(const QString &key, const QVariant &value) {
    if (this->Config.isNull() || !this->Config.isObject())
        return {false, {}, "Конфигурация не инициализирована"};

    QJsonObject obj = this->Config.object();

    if (!obj.contains(key))
        return {false, {}, "Поле \"" + key + "\" не найдено"};

    // Конвертация QVariant → QJsonValue
    QJsonValue newValue;

    switch (value.typeId()) {
        case QMetaType::QString:
            newValue = QJsonValue(value.toString());
            break;
        case QMetaType::Bool:
            newValue = QJsonValue(value.toBool());
            break;
        case QMetaType::Double:
        case QMetaType::Int:
            newValue = QJsonValue(value.toDouble());
            break;
        case QMetaType::QJsonArray:
            newValue = value.toJsonArray();
            break;
        case QMetaType::QStringList:
            newValue = value.toJsonArray();
            break;
        case QMetaType::QVariantList:
            newValue = value.toJsonArray();
            break;
        default:
            return {false, {}, "Неподдерживаемый тип QVariant для записи"};
    }

    obj[key] = newValue;
    this->Config.setObject(obj);

    saveConfig();
    return {true, {}, ""};
}

/* ============================================================
 * API KEY
 * ============================================================ */

Result ChatGPTConfig::getApiKey() {
    auto pathField = getConfigField("apiKeyPath");
    if (!pathField.success)
        return pathField;

    QString path = pathField.data.toString();

    // "~" → /home/user
    if (path.startsWith("~"))
        path.replace(0, 1, QDir::homePath());

    QFile file(path);
    if (!file.exists())
        return {false, {}, "Файл API ключа не найден: " + path};

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {false, {}, "Не удалось открыть файл: " + path};

    QString key = QString::fromUtf8(file.readAll()).trimmed();
    file.close();

    if (key.isEmpty())
        return {false, {}, "Файл существует, но ключ пуст"};

    return {true, key, ""};
}

Result ChatGPTConfig::setApiKey(const QString &apiKey) {
    auto pathField = getConfigField("apiKeyPath");
    if (!pathField.success)
        return pathField;

    QString path = pathField.data.toString();

    if (path.startsWith("~"))
        path.replace(0, 1, QDir::homePath());

    QFileInfo info(path);
    QDir dir(info.absolutePath());

    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return {false, {}, "Не удалось создать директорию: " + info.absolutePath()};
        }
    }

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return {false, {}, "Ошибка записи в файл: " + path};

    QTextStream out(&file);
    out << apiKey.trimmed();
    file.close();

    return {true, {"API ключ успешно записан в: " + path}, ""};
}

Result ChatGPTConfig::setApiKeyPath(const QString &path) {
    return setConfigField("apiKeyPath", path);
}

Result ChatGPTConfig::getApiKeyPath() {
    auto chf = getConfigField("apiKeyPath");
    return getConfigField("apiKeyPath");
}

/* ============================================================
 * TOR
 * ============================================================ */

Result ChatGPTConfig::getTorEnable() const {
    return getConfigField("torEnabled");
}

Result ChatGPTConfig::setTorEnable(bool enabled) {
    return setConfigField("torEnabled", enabled);
}

/* ============================================================
 * LOAD / SAVE CONFIG
 * ============================================================ */

Result ChatGPTConfig::loadConfig() {
    const QString configPath = directoryPath + "/Config.json";
    QFile file(configPath);

    QDir dir(directoryPath);
    if (!dir.exists())
        dir.mkpath(".");

    // Создать новый конфиг
    if (!file.exists()) {
        QJsonObject defaultConfig{
            {"apiKeyPath", "~/.config/chatgpt/api.key"},
            {"currentModel", ""},
            {"selectedModel", QJsonArray{}},
            {"torEnabled", false}
        };

        QJsonDocument doc(defaultConfig);
        QFile f(configPath);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return {false, {}, "Не удалось создать Config.json"};

        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
        this->Config = doc;
        return {true, doc.toJson(), ""};
    }

    // Чтение существующего
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {false, {}, "Не удалось открыть Config.json"};

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error != QJsonParseError::NoError)
        return {false, {}, "Ошибка JSON: " + err.errorString()};

    this->Config = doc;
    return {true, doc.toJson(), ""};
}

Result ChatGPTConfig::saveConfig() {
    QString configPath = directoryPath + "/Config.json";
    QFile file(configPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return {false, {}, "Не удалось сохранить Config.json"};

    file.write(this->Config.toJson(QJsonDocument::Indented));
    file.close();

    return {true, {}, ""};
}

/* ============================================================
 * MODEL
 * ============================================================ */

Result ChatGPTConfig::getCurrentModel() const {
    return getConfigField("currentModel");
}

Result ChatGPTConfig::setCurrentModel(const QString &str) {
    return setConfigField("currentModel", str);
}

/* ============================================================
 * selected-MODEL
 * ============================================================ */
Result ChatGPTConfig::getSelectedModels() {
    return getConfigField("selectedModel");
}

Result ChatGPTConfig::addSelectedModels(const QString &model) {
    // Получаем поле selectedModel
    auto field = getConfigField("selectedModel");
    if (!field.success) {
        return field;
    }

    QJsonValue val = field.data.toJsonValue();

    // Проверяем, что это массив
    if (!val.isArray()) {
        return { false, {}, "Поле selectedModel имеет неверный формат (ожидается массив)" };
    }

    QJsonArray arr = val.toArray();

    // Проверяем, нет ли уже модели
    for (const auto &item : arr) {
        if (item.toString() == model) {
            return { false, {}, "Модель уже есть в списке" };
        }
    }
    // Добавляем
    arr.append(model);
    // Сохраняем
    return setConfigField("selectedModel", arr);
}

Result ChatGPTConfig::delSelectedModels(const QString &model) {
    // Получаем поле
    auto result = getConfigField("selectedModel");
    if (!result.success) {
        return { false, {}, "Поле selectedModel не найдено" };
    }

    QVariant data = result.data;

    if (data.type() != 47) {
        return { false, {}, "Поле selectedModel НЕ QVariantList (тип 47)" };
    }

    QVariantList list = data.toList();

    // Удаление элемента
    bool removed = false;
    for (int i = 0; i < list.size(); i++) {
        if (list[i].toString() == model) {
            list.removeAt(i);
            removed = true;
            break;
        }
    }

    if (!removed) {
        return { false, list, "Модель не найдена в selectedModel" };
    }

    // Сохраняем массив обратно в конфиг
    return setConfigField("selectedModel", list);
}


