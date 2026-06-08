#include "ProjectTreeModel.h"
#include "ProjectTreeItem.h"
#include <QDebug>

ProjectTreeModel::ProjectTreeModel(
    ProjectTreeItem* root,
    ProjectContextService* service,
    QObject* parent)
    : QAbstractItemModel(parent),
    m_root(root),
    m_service(service)
{
    // loadTestData();
}

// Индекс элемента по row/column
QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    auto* parentItem = !parent.isValid()
                           ? m_root
                           : static_cast<ProjectTreeItem*>(parent.internalPointer());

    auto* childItem = parentItem->child(row);
    if (!childItem)
        return QModelIndex();

    return createIndex(row, column, childItem);
}

// Родитель элемента
QModelIndex ProjectTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    auto* childItem = static_cast<ProjectTreeItem*>(index.internalPointer());
    auto* parentItem = childItem->parentItem();

    if (!parentItem || parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

// Количество детей
int ProjectTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    auto* parentItem = !parent.isValid()
                           ? m_root
                           : static_cast<ProjectTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ProjectTreeModel::columnCount(const QModelIndex&) const
{
    return 1;
}

// Данные по роли
QVariant ProjectTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    auto* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    switch (role)
    {
    case NameRole:
        return item->name();

    case PathRole:
        return item->path();

    case TypeRole:
        return item->type() == ProjectTreeItem::Type::Folder
                   ? "folder"
                   : "file";

    case Qt::CheckStateRole:
        return item->selected()
                   ? Qt::Checked
                   : Qt::Unchecked;
    }

    return {};
}

// Установка данных (выбор/selected)
bool ProjectTreeModel::setData(
    const QModelIndex& index,
    const QVariant& value,
    int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::CheckStateRole)
        return false;

    auto* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    const auto state = static_cast<Qt::CheckState>(value.toInt());
    const bool checked = (state == Qt::Checked);

    item->setSelected(checked);

    // 🔥 обновляем UI
    emit dataChanged(index, index, {Qt::CheckStateRole});

    return true;
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled
           | Qt::ItemIsSelectable
           | Qt::ItemIsUserCheckable
           | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ProjectTreeModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {PathRole, "path"},
        {TypeRole, "type"},


        {Qt::CheckStateRole, "checkState"}
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

void ProjectTreeModel::clearTree()
{
    delete m_root;

    m_root = new ProjectTreeItem(
        "",
        "",
        ProjectTreeItem::Type::Folder);
}

ProjectTreeItem* ProjectTreeModel::createItemTree(
    const ProjectNode& node,
    ProjectTreeItem* parent)
{
    auto* item = new ProjectTreeItem(
        node.name,
        node.path,
        node.isDir
            ? ProjectTreeItem::Type::Folder
            : ProjectTreeItem::Type::File,
        parent);

    item->setSelected(node.selected);

    for (const auto& child : node.children)
    {
        item->appendChild(
            createItemTree(child, item));
    }

    return item;
}

void ProjectTreeModel::loadFolder(const QString& path)
{
    const auto localPath = QUrl(path).toLocalFile();
    const auto nodes = m_service->buildTree(localPath);

    beginResetModel();

    m_root->clearChildren();

    for (const auto& node : nodes)
    {
        m_root->appendChild(createItemTree(node, m_root));
    }

    endResetModel();
}

void ProjectTreeModel::loadFile(const QString& path)
{
    const auto localPath = QUrl(path).toLocalFile();
    QFileInfo info(localPath);

    beginResetModel();

    m_root->clearChildren();

    m_root->appendChild(
        new ProjectTreeItem(
            info.fileName(),
            localPath,
            ProjectTreeItem::Type::File,
            m_root));

    endResetModel();
}

QStringList ProjectTreeModel::selectedFiles() const
{
    QStringList result;
    collectSelected(m_root, result);
    return result;
}

void ProjectTreeModel::collectSelected(ProjectTreeItem* item, QStringList& out) const
{
    // if (!item)
    //     return;

    // if (item->type() == ProjectTreeItem::Type::File && item->selected())
    //     out.push_back(item->path());

    // for (auto* child : item->children())
    //     collectSelected(child, out);
}