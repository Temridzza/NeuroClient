#include "ProjectTreeItem.h"

ProjectTreeItem::ProjectTreeItem(
    const QString& name,
    const QString& path,
    Type type,
    ProjectTreeItem* parent)
    : m_name(name),
    m_path(path),
    m_type(type),
    m_parent(parent)
{
}

void ProjectTreeItem::appendChild(ProjectTreeItem* child)
{
    if (child) {
        m_children.append(child);
    }
}

ProjectTreeItem* ProjectTreeItem::child(int row) const
{
    if (row < 0 || row >= m_children.size())
        return nullptr;
    return m_children.at(row);
}

int ProjectTreeItem::childCount() const
{
    return m_children.size();
}

int ProjectTreeItem::row() const
{
    if (!m_parent)
        return 0;
    return m_parent->m_children.indexOf(const_cast<ProjectTreeItem*>(this));
}

ProjectTreeItem* ProjectTreeItem::parentItem() const
{
    return m_parent;
}

QString ProjectTreeItem::name() const
{
    return m_name;
}

QString ProjectTreeItem::path() const
{
    return m_path;
}

bool ProjectTreeItem::selected() const
{
    return m_selected;
}

void ProjectTreeItem::setSelected(bool value)
{
    m_selected = value;

    // Если папка, можно рекурсивно отмечать всех детей
    if (m_type == Type::Folder) {
        for (auto child : m_children) {
            child->setSelected(value);
        }
    }
}

ProjectTreeItem::Type ProjectTreeItem::type() const
{
    return m_type;
}