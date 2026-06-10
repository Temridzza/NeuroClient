#ifndef PROJECTNODE_H
#define PROJECTNODE_H

#include <QString>
#include <QVector>

// ------------------ структура узла ------------------
struct ProjectNode
{
    QString name;
    QString path;        // относительный путь от корня проекта
    bool isDir = false;
    bool selected = false;

    int estimatedTokens = 0;

    QVector<ProjectNode> children;
};

#endif // PROJECTNODE_H
