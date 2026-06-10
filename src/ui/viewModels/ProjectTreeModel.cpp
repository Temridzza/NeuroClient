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
    case TokensRole:
        return item->estimatedTokens();

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

    if (checked)
    {
        calculateTokens(item);
        countSelectedFiles(item);
        calculateSizes(item);
    }

    // 🔥 обновляем UI
    emit dataChanged(
        index,
        index,
        {Qt::CheckStateRole, TokensRole});

    emit selectedSizeChanged();
    emit selectedFilesCountChanged();
    emit selectedTokensChanged();

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
        { TokensRole, "tokens" },


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
    m_root->setPath(m_service->path());

    for (const auto& node : nodes)
    {
        m_root->appendChild(createItemTree(node, m_root));
    }
    emit pathChanged();
    emit selectedTokensChanged();
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

    emit selectedTokensChanged();
    endResetModel();
}

QStringList ProjectTreeModel::selectedFiles() const
{
    QStringList result;
    collectSelected(m_root, result);
    return result;
}

void ProjectTreeModel::collectSelected(
    ProjectTreeItem* item,
    QStringList& out) const
{
    if (!item)
        return;

    if (item->type() == ProjectTreeItem::Type::File &&
        item->selected())
    {
        out.push_back(item->path());
    }

    for (auto* child : item->children())
    {
        collectSelected(child, out);
    }
}

ProjectContextData ProjectTreeModel::buildContext() const
{
    ProjectContextData ctx;

    for (auto* child : m_root->children())
    {
        buildTreeText(child, ctx.tree, 0);
    }

    QStringList selected;
    collectSelected(m_root, selected);

    ctx.files =
        m_service->collectFiles(selected.toVector());

    return ctx;
}

void ProjectTreeModel::buildTreeText(
    ProjectTreeItem* item,
    QString& out,
    int depth) const
{
    if (!item)
        return;

    out += QString(depth * 2, ' ');
    out += item->name();
    out += '\n';

    for (auto* child : item->children())
    {
        buildTreeText(child, out, depth + 1);
    }
}

void ProjectTreeModel::collectSelectedFiles(
    ProjectTreeItem* item,
    QString& out) const
{
    QStringList selected;
    collectSelected(item, selected);

    out += m_service->collectFiles(
        selected.toVector());
}

QString ProjectTreeModel::path() const {
    return m_root->path();
}

int ProjectTreeModel::selectedTokens() const
{
    return collectTokens(m_root);
}

int ProjectTreeModel::collectTokens(
    ProjectTreeItem* item) const
{
    int total = 0;

    if (item->selected() &&
        item->type() == ProjectTreeItem::Type::File)
    {
        total += item->estimatedTokens();
    }

    for (auto* child : item->children())
    {
        total += collectTokens(child);
    }

    return total;
}

void ProjectTreeModel::calculateTokens(ProjectTreeItem* item)
{
    if (!item)
        return;

    if (item->type() == ProjectTreeItem::Type::File &&
        item->estimatedTokens() == 0)
    {
        item->setEstimatedTokens(
            m_service->estimateTokens(item->path()));
    }

    for (auto* child : item->children())
    {
        calculateTokens(child);
    }
}

int ProjectTreeModel::selectedFilesCount() const
{
    return countSelectedFiles(m_root);
}

int ProjectTreeModel::countSelectedFiles(ProjectTreeItem* item) const
{
    int count = 0;

    if (item->type() == ProjectTreeItem::Type::File &&
        item->selected())
    {
        ++count;
    }

    for (auto* child : item->children())
    {
        count += countSelectedFiles(child);
    }

    return count;
}

qint64 ProjectTreeModel::selectedSize() const
{
    return collectSize(m_root);
}

qint64 ProjectTreeModel::collectSize(
    ProjectTreeItem* item) const
{
    qint64 total = 0;

    if (item->selected() &&
        item->type() == ProjectTreeItem::Type::File)
    {
        total += item->fileSize();
    }

    for (auto* child : item->children())
    {
        total += collectSize(child);
    }

    return total;
}

void ProjectTreeModel::calculateSizes(ProjectTreeItem* item)
{
    if (!item)
        return;

    if (item->type() == ProjectTreeItem::Type::File &&
        item->fileSize() == 0)
    {
        item->setFileSize(
            m_service->estimateFileSize(
                item->path()));
    }

    for (auto* child : item->children())
    {
        calculateSizes(child);
    }
}