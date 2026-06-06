#include "ProjectTreeModel.h"
#include "ProjectTreeItem.h"
#include <QDebug>

ProjectTreeModel::ProjectTreeModel(ProjectTreeItem* root, QObject* parent)
    : QAbstractItemModel(parent),
    m_root(root)

{
    loadTestData();
}

// Индекс элемента по row/column
QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ProjectTreeItem* parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    ProjectTreeItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

// Родитель элемента
QModelIndex ProjectTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    ProjectTreeItem* childItem = static_cast<ProjectTreeItem*>(index.internalPointer());
    ProjectTreeItem* parentItem = childItem->parentItem();

    if (!parentItem || parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

// Количество детей
int ProjectTreeModel::rowCount(const QModelIndex& parent) const
{
    ProjectTreeItem* parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ProjectTreeModel::columnCount(const QModelIndex&) const {
    return 1; // только имя
}

// Данные по роли
QVariant ProjectTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ProjectTreeItem* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    switch (role)
    {
    case NameRole: return item->name();
    case PathRole: return item->path();
    case TypeRole: return item->type() == ProjectTreeItem::Type::Folder ? "folder" : "file";
    case SelectedRole: return item->selected();
    }

    return QVariant();
}

// Установка данных (выбор/selected)
bool ProjectTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    ProjectTreeItem* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    if (role == SelectedRole) {
        item->setSelected(value.toBool());
        emit dataChanged(index, index, {SelectedRole});
        return true;
    }

    return false;
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

QHash<int, QByteArray> ProjectTreeModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {PathRole, "path"},
        {TypeRole, "type"},
        {SelectedRole, "selected"}
    };
}

// Тестовые данные
void ProjectTreeModel::loadTestData()
{
    auto src = new ProjectTreeItem("src", "src", ProjectTreeItem::Type::Folder, m_root);
    src->appendChild(new ProjectTreeItem("main.cpp", "src/main.cpp", ProjectTreeItem::Type::File, src));
    src->appendChild(new ProjectTreeItem("utils.cpp", "src/utils.cpp", ProjectTreeItem::Type::File, src));

    auto assets = new ProjectTreeItem("assets", "assets", ProjectTreeItem::Type::Folder, m_root);
    assets->appendChild(new ProjectTreeItem("logo.svg", "assets/logo.svg", ProjectTreeItem::Type::File, assets));

    m_root->appendChild(src);
    m_root->appendChild(assets);
    m_root->appendChild(new ProjectTreeItem("README.md", "README.md", ProjectTreeItem::Type::File, m_root));

    // уведомляем QML
    beginResetModel();
    endResetModel();
}