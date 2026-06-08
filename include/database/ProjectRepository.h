#ifndef PROJECTREPOSITORY_H
#define PROJECTREPOSITORY_H

#pragma once

#include <QSqlDatabase>
#include <QString>

#include <optional>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

#include "ChatProject.h"

class ProjectRepository
{
public:
    explicit ProjectRepository(QSqlDatabase db);

    bool save(const ChatProject& project);

    std::optional<ChatProject> findByChatId(
        const QString& chatId) const;

    bool exists(
        const QString& chatId) const;

    bool remove(
        const QString& chatId);

private:
    QSqlDatabase m_db;
};

#endif // PROJECTREPOSITORY_H
