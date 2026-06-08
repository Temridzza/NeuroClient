#include "ProjectService.h"

ProjectService::ProjectService(
    ProjectRepository* repository)
    : m_repository(repository)
{
}

bool ProjectService::setProjectPath(
    const QString& chatId,
    const QString& projectPath)
{
    ChatProject project {
        .chatId = chatId,
        .projectPath = projectPath
    };

    return m_repository->save(project);
}

std::optional<QString>
ProjectService::projectPath(
    const QString& chatId) const
{
    auto project = m_repository->findByChatId(chatId);

    if (!project) {
        return std::nullopt;
    }

    return project->projectPath;
}

bool ProjectService::hasProject(
    const QString& chatId) const
{
    return m_repository->exists(chatId);
}

bool ProjectService::removeProject(
    const QString& chatId)
{
    return m_repository->remove(chatId);
}