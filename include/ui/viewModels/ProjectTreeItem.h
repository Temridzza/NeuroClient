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

    ~ProjectTreeItem();

    void clearChildren();

    QVector<ProjectTreeItem *> &children();
    void setPath(const QString& path);

    int estimatedTokens() const;
    void setEstimatedTokens(int tokens);

    qint64 fileSize() const;
    void setFileSize(qint64 size);

private:
    QString m_name;
    QString m_path;

    bool m_selected = false;

    Type m_type;

    ProjectTreeItem* m_parent = nullptr;
    QVector<ProjectTreeItem*> m_children;

    int m_estimatedTokens = 0;
    qint64 m_fileSize = 0;

};
