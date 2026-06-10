#ifndef PROJECTCONTEXTSERVICE_H
#define PROJECTCONTEXTSERVICE_H

#include "ProjectNode.h"
#include <QVector>
#include <QString>
#include <QPair>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QUrl>

#include <QDebug>

class ProjectContextService
{
public:
    ProjectContextService();

    // строит дерево проекта
    QVector<ProjectNode> buildTree(const QString& projectPath);

    // получает массив относительных путей и добавляет их содержимое в "промпт"
    QString collectFiles(const QVector<QString>& relativePaths);
    QString path() const;

    int estimateTokens(const QString& relativePath) const;
    qint64 estimateFileSize(const QString& filePath) const;

private:
    ProjectNode buildNode(const QString& absolutePath);

    void appendFilesToPrompt(const QVector<QPair<QString, QString>>& files);

private:
    QString m_projectRoot;

    void openPath(const QString& path);
};

#endif // PROJECTCONTEXTSERVICE_H
