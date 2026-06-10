#include "ProjectContextService.h"


ProjectContextService::ProjectContextService()
{
}

// ------------------ buildTree ------------------
QVector<ProjectNode> ProjectContextService::buildTree(const QString& projectPath)
{
    m_projectRoot = projectPath;
    QVector<ProjectNode> result;

    QDir root(projectPath);

    auto entries = root.entryInfoList(
        QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
        QDir::DirsFirst | QDir::Name);

    for (const QFileInfo& entry : entries)
    {
        result.append(
            buildNode(entry.absoluteFilePath()));
    }

    return result;
}

// ------------------ buildNode ------------------
ProjectNode ProjectContextService::buildNode(const QString& absolutePath)
{
    QFileInfo info(absolutePath);

    ProjectNode node;
    node.name = info.fileName();
    node.path = QDir(m_projectRoot).relativeFilePath(absolutePath);
    node.isDir = info.isDir();
    node.selected = false;

    if (info.isDir()) {
        QDir dir(absolutePath);
        QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                                                  QDir::DirsFirst | QDir::Name);
        for (const QFileInfo& child : entries)
            node.children.append(buildNode(child.absoluteFilePath()));
    }

    return node;
}

// ------------------ collectFiles ------------------
QString ProjectContextService::collectFiles(
    const QVector<QString>& relativePaths)
{
    QString result;

    for (const QString& relPath : relativePaths)
    {
        QString absolutePath =
            QDir(m_projectRoot).filePath(relPath);

        QFile file(absolutePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning()
            << "Не удалось открыть файл:"
            << absolutePath;
            continue;
        }

        result += "\n=== FILE: ";
        result += relPath;
        result += " ===\n";
        result += QString::fromUtf8(file.readAll());
        result += "\n";
    }

    return result;
}

int ProjectContextService::estimateTokens(
    const QString& relativePath) const
{
    const QString absolutePath =
        QDir(m_projectRoot).filePath(relativePath);

    QFile file(absolutePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }

    const QString text =
        QString::fromUtf8(file.readAll());

    return static_cast<int>(
        std::ceil(text.size() / 3.5));
}

// ------------------ appendFilesToPrompt ------------------
void ProjectContextService::appendFilesToPrompt(const QVector<QPair<QString, QString>>& files)
{
    for (const auto& [path, content] : files) {
        qDebug() << "FILE:" << path;
        qDebug() << content.left(200); // первые 200 символов
    }
}

void ProjectContextService::openPath(const QString& path)
{
    QUrl url(path);

    QString localPath = url.toLocalFile();

    qDebug() << "Selected:" << localPath;

    QFileInfo info(localPath);

    if (info.isDir()) {
        auto tree = buildTree(localPath);
        // emit projectLoaded(tree);
    } else {
        collectFiles({localPath});
    }
}

QString ProjectContextService::path() const {
    return m_projectRoot;
}

qint64 ProjectContextService::estimateFileSize(
    const QString& relativePath) const
{
    const QString absolutePath =
        QDir(m_projectRoot).filePath(relativePath);

    QFileInfo info(absolutePath);

    if (!info.exists() || !info.isFile())
    {
        return 0;
    }

    return info.size();
}