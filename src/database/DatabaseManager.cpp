#include "DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>


DatabaseManager::DatabaseManager() {}

bool DatabaseManager::init(const QString& path) {
    if (QSqlDatabase::contains("main")) {
        m_db = QSqlDatabase::database("main");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "main");
        m_db.setDatabaseName(path);
    }

    if (!m_db.open()) {
        qWarning() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }

    if (!createSchema()) {
        return false;
    }

    seed();
    return true;
}

QSqlDatabase& DatabaseManager::db() {
    return m_db;
}

bool DatabaseManager::createSchema() {
    QSqlQuery q(m_db);

    q.exec("PRAGMA foreign_keys = ON");

    if (!q.exec(R"(
        CREATE TABLE IF NOT EXISTS providers (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            base_url TEXT NOT NULL,
            api_key TEXT,
            created_at INTEGER NOT NULL,
            updated_at INTEGER NOT NULL
        )
    )")) {
        qWarning() << q.lastError();
        return false;
    }

    if (!q.exec(R"(
        CREATE TABLE IF NOT EXISTS models (
            id TEXT PRIMARY KEY,
            provider_id TEXT NOT NULL,
            model_key TEXT NOT NULL,
            display_name TEXT NOT NULL,
            api_key_override TEXT,
            context_window INTEGER,
            max_output_tokens INTEGER,
            enabled INTEGER NOT NULL DEFAULT 1,
            created_at INTEGER NOT NULL,
            updated_at INTEGER NOT NULL,
            FOREIGN KEY(provider_id)
                REFERENCES providers(id)
                ON DELETE CASCADE,
            UNIQUE(provider_id, model_key)
        )
    )")) {
        qWarning() << q.lastError();
        return false;
    }

    if (!q.exec(R"(
        CREATE TABLE IF NOT EXISTS chats (
            id TEXT PRIMARY KEY,
            title TEXT,
            root_message_id TEXT,
            created_at INTEGER NOT NULL,
            updated_at INTEGER NOT NULL
        )
    )")) {
        qWarning() << q.lastError();
        return false;
    }

    if (!q.exec(R"(
        CREATE TABLE IF NOT EXISTS messages (
            id TEXT PRIMARY KEY,
            chat_id TEXT NOT NULL,
            parent_id TEXT,
            role TEXT NOT NULL,
            content TEXT NOT NULL,
            provider_id TEXT,
            model_id TEXT,
            provider_name_snapshot TEXT,
            model_name_snapshot TEXT,
            temperature REAL,
            top_p REAL,
            max_tokens INTEGER,
            prompt_tokens INTEGER,
            completion_tokens INTEGER,
            active_child_id TEXT,
            deleted INTEGER NOT NULL DEFAULT 0,
            created_at INTEGER NOT NULL,
            FOREIGN KEY(chat_id) REFERENCES chats(id) ON DELETE CASCADE,
            FOREIGN KEY(parent_id) REFERENCES messages(id) ON DELETE CASCADE,
            FOREIGN KEY(active_child_id) REFERENCES messages(id) ON DELETE SET NULL,
            FOREIGN KEY(provider_id) REFERENCES providers(id) ON DELETE SET NULL,
            FOREIGN KEY(model_id) REFERENCES models(id) ON DELETE SET NULL
        )
    )")) {
        qWarning() << q.lastError();
        return false;
    }

    q.exec("CREATE INDEX IF NOT EXISTS idx_models_provider_id ON models(provider_id)");
    q.exec("CREATE INDEX IF NOT EXISTS idx_messages_chat_id ON messages(chat_id)");
    q.exec("CREATE INDEX IF NOT EXISTS idx_messages_parent_id ON messages(parent_id)");
    q.exec("CREATE INDEX IF NOT EXISTS idx_messages_model_id ON messages(model_id)");
    q.exec("CREATE INDEX IF NOT EXISTS idx_messages_provider_id ON messages(provider_id)");
    q.exec("CREATE INDEX IF NOT EXISTS idx_messages_created_at ON messages(created_at)");
    q.exec("CREATE INDEX IF NOT EXISTS idx_chats_updated_at ON chats(updated_at)");

    return true;
}

bool DatabaseManager::seed() {
    QSqlQuery q(m_db);

    // проверяем, есть ли openrouter
    q.prepare("SELECT id FROM providers WHERE name = ?");
    q.addBindValue("openrouter");

    if (!q.exec()) {
        qWarning() << q.lastError();
        return false;
    }

    QString providerId;

    if (q.next()) {
        providerId = q.value(0).toString();
    } else {
        providerId = "openrouter";

        QSqlQuery insertProvider(m_db);
        insertProvider.prepare(R"(
            INSERT INTO providers (
                id, name, base_url, api_key, created_at, updated_at
            ) VALUES (?, ?, ?, ?, ?, ?)
        )");

        const qint64 now = QDateTime::currentSecsSinceEpoch();

        insertProvider.addBindValue(providerId);
        insertProvider.addBindValue("openrouter");
        insertProvider.addBindValue("https://openrouter.ai/api/v1/chat/completions");
        insertProvider.addBindValue("paste");
        insertProvider.addBindValue(now);
        insertProvider.addBindValue(now);

        if (!insertProvider.exec()) {
            qWarning() << insertProvider.lastError();
            return false;
        }
    }

    // helper lambda для моделей
    auto insertModel = [&](const QString& id,
                           const QString& modelKey,
                           const QString& displayName) {

        QSqlQuery m(m_db);
        m.prepare(R"(
            INSERT OR IGNORE INTO models (
                id,
                provider_id,
                model_key,
                display_name,
                created_at,
                updated_at,
                enabled
            ) VALUES (?, ?, ?, ?, ?, ?, 1)
        )");

        const qint64 now = QDateTime::currentSecsSinceEpoch();

        m.addBindValue(id);
        m.addBindValue(providerId);
        m.addBindValue(modelKey);
        m.addBindValue(displayName);
        m.addBindValue(now);
        m.addBindValue(now);

        if (!m.exec()) {
            qWarning() << m.lastError();
        }
    };

    insertModel(
        "openrouter-gpt-oss-20b",
        "openai/gpt-oss-20b:free",
        "GPT OSS 20B (Free)"
        );

    insertModel(
        "openrouter-nemotron-3-nano",
        "nvidia/nemotron-3-nano-30b-a3b:free",
        "Nemotron 3 Nano 30B (Free)"
        );

    return true;
}