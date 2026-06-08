#ifndef PROJECTSERVICE_H
#define PROJECTSERVICE_H

#pragma once

#include <QString>
#include <optional>

#include "ProjectRepository.h"

class ProjectService
{
public:
    explicit ProjectService(ProjectRepository *repository);

    bool setProjectPath(
        const QString& chatId,
        const QString& projectPath);

    std::optional<QString> projectPath(
        const QString& chatId) const;

    bool hasProject(
        const QString& chatId) const;

    bool removeProject(
        const QString& chatId);

private:
    ProjectRepository* m_repository;
};

#endif // PROJECTSERVICE_H
