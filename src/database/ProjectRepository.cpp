#include "ProjectRepository.h"

ProjectRepository::ProjectRepository(QSqlDatabase db)
    : m_db(std::move(db))
{
}

bool ProjectRepository::save(
    const ChatProject& project)
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        INSERT OR REPLACE INTO chat_projects (
            chat_id,
            project_path
        )
        VALUES (?, ?)
    )");

    q.addBindValue(project.chatId);
    q.addBindValue(project.projectPath);

    if (!q.exec()) {
        qWarning() << q.lastError();
        return false;
    }

    return true;
}

std::optional<ChatProject>
ProjectRepository::findByChatId(
    const QString& chatId) const
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT
            chat_id,
            project_path
        FROM chat_projects
        WHERE chat_id = ?
    )");

    q.addBindValue(chatId);

    if (!q.exec()) {
        qWarning() << q.lastError();
        return std::nullopt;
    }

    if (!q.next()) {
        return std::nullopt;
    }

    ChatProject project;

    project.chatId = q.value("chat_id").toString();
    project.projectPath = q.value("project_path").toString();

    return project;
}

bool ProjectRepository::exists(
    const QString& chatId) const
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        SELECT EXISTS(
            SELECT 1
            FROM chat_projects
            WHERE chat_id = ?
        )
    )");

    q.addBindValue(chatId);

    if (!q.exec()) {
        qWarning() << q.lastError();
        return false;
    }

    return q.next() && q.value(0).toBool();
}

bool ProjectRepository::remove(
    const QString& chatId)
{
    QSqlQuery q(m_db);

    q.prepare(R"(
        DELETE FROM chat_projects
        WHERE chat_id = ?
    )");

    q.addBindValue(chatId);

    if (!q.exec()) {
        qWarning() << q.lastError();
        return false;
    }

    return true;
}