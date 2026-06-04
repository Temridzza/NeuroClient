#include "DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>


DatabaseManager::DatabaseManager() {}

bool DatabaseManager::init(const QString& path)
{
    qDebug() << "DB path:" << path;

    if (QSqlDatabase::contains("main")) {
        qDebug() << "Using existing connection";
        m_db = QSqlDatabase::database("main");
    } else {
        qDebug() << "Creating new connection";
        m_db = QSqlDatabase::addDatabase("QSQLITE", "main");
        m_db.setDatabaseName(path);
    }

    if (!m_db.open()) {
        qWarning() << "Failed to open database:"
                   << m_db.lastError().text();
        return false;
    }

    qDebug() << "Database opened";

    if (!createSchema()) {
        qWarning() << "createSchema failed";
        return false;
    }

    qDebug() << "Schema created";

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

            prompt TEXT,

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

            prompt TEXT,

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

            provider_id TEXT,
            model_id TEXT,

            prompt TEXT,

            root_message_id TEXT,
            created_at INTEGER NOT NULL,
            updated_at INTEGER NOT NULL,

            FOREIGN KEY(provider_id)
                REFERENCES providers(id)
                ON DELETE SET NULL,

            FOREIGN KEY(model_id)
                REFERENCES models(id)
                ON DELETE SET NULL
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
            content TEXT,
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

    // ===============================
    // OpenRouter provider
    // ===============================
    q.prepare("SELECT id FROM providers WHERE name = ?");
    q.addBindValue("openrouter");

    if (!q.exec()) {
        qWarning() << q.lastError();
        return false;
    }

    QString openrouterId;

    if (q.next()) {
        openrouterId = q.value(0).toString();
    } else {
        openrouterId = "openrouter";

        QSqlQuery insertProvider(m_db);
        insertProvider.prepare(R"(
            INSERT INTO providers (
                id, name, base_url, api_key, created_at, updated_at
            ) VALUES (?, ?, ?, ?, ?, ?)
        )");

        const qint64 now = QDateTime::currentSecsSinceEpoch();

        insertProvider.addBindValue(openrouterId);
        insertProvider.addBindValue("openrouter");
        insertProvider.addBindValue("https://openrouter.ai/api/v1/chat/completions");
        insertProvider.addBindValue("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");
        insertProvider.addBindValue(now);
        insertProvider.addBindValue(now);

        if (!insertProvider.exec()) {
            qWarning() << insertProvider.lastError();
            return false;
        }
    }

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
        m.addBindValue(openrouterId);
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

    // ===============================
    // Local provider (LM Studio / OpenAI-compatible)
    // ===============================
    QString localProviderId;

    QSqlQuery localCheck(m_db);
    localCheck.prepare("SELECT id FROM providers WHERE name = ?");
    localCheck.addBindValue("local");

    if (localCheck.exec() && localCheck.next()) {
        localProviderId = localCheck.value(0).toString();
    } else {
        localProviderId = "local";

        QSqlQuery insertProvider(m_db);
        insertProvider.prepare(R"(
            INSERT INTO providers (
                id, name, base_url, api_key, created_at, updated_at
            ) VALUES (?, ?, ?, ?, ?, ?)
        )");

        const qint64 now = QDateTime::currentSecsSinceEpoch();

        insertProvider.addBindValue(localProviderId);
        insertProvider.addBindValue("local");
        insertProvider.addBindValue("http://localhost:1234/v1/chat/completions");
        insertProvider.addBindValue(""); // LM Studio обычно без ключа
        insertProvider.addBindValue(now);
        insertProvider.addBindValue(now);

        if (!insertProvider.exec()) {
            qWarning() << insertProvider.lastError();
            return false;
        }
    }

    auto insertLocalModel = [&](const QString& id,
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
        m.addBindValue(localProviderId);
        m.addBindValue(modelKey);
        m.addBindValue(displayName);
        m.addBindValue(now);
        m.addBindValue(now);

        if (!m.exec()) {
            qWarning() << m.lastError();
        }
    };

    insertLocalModel(
        "local-qwen2.5-coder",
        "qwen2.5-coder-1.5b-instruct",
        "Qwen2.5 Coder 1.5B (Local)"
        );

    return true;
}