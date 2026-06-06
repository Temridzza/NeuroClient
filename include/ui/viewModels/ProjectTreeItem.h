#pragma once

#include <QString>
#include <QVector>

class ProjectTreeItem
{
public:
    enum class Type
    {
        File,
        Folder
    };

    explicit ProjectTreeItem(
        const QString& name,
        const QString& path,
        Type type,
        ProjectTreeItem* parent = nullptr);

    void appendChild(ProjectTreeItem* child);

    ProjectTreeItem* child(int row) const;
    int childCount() const;

    int row() const;

    ProjectTreeItem* parentItem() const;

    QString name() const;
    QString path() const;

    bool selected() const;
    void setSelected(bool value);

    Type type() const;

private:
    QString m_name;
    QString m_path;

    bool m_selected = false;

    Type m_type;

    ProjectTreeItem* m_parent = nullptr;
    QVector<ProjectTreeItem*> m_children;
};
