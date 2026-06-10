#ifndef PROJECTTREEMODEL_H
#define PROJECTTREEMODEL_H

#pragma once

#include <QAbstractItemModel>
#include "ProjectTreeItem.h"
#include "ProjectContextService.h"

struct ProjectContextData
{
    QString tree;
    QString files;
};

class ProjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(
        int selectedTokens
            READ selectedTokens
                NOTIFY selectedTokensChanged
        )
    Q_PROPERTY(
        int selectedFilesCount
            READ selectedFilesCount
                NOTIFY selectedFilesCountChanged
        )
    Q_PROPERTY(
        qint64 selectedSize
            READ selectedSize
                NOTIFY selectedSizeChanged)

public:
    enum Roles
    {
        NameRole = Qt::UserRole + 1,
        PathRole,
        TypeRole,
        SelectedRole,
        TokensRole
    };

    explicit ProjectTreeModel(
        ProjectTreeItem* root,
        ProjectContextService* service,
        QObject* parent = nullptr);

    QModelIndex index(
        int row,
        int column,
        const QModelIndex& parent) const override;

    QModelIndex parent(
        const QModelIndex& index) const override;

    int rowCount(
        const QModelIndex& parent) const override;

    int columnCount(
        const QModelIndex& parent) const override;

    QVariant data(
        const QModelIndex& index,
        int role) const override;

    bool setData(
        const QModelIndex& index,
        const QVariant& value,
        int role) override;

    Qt::ItemFlags flags(
        const QModelIndex& index) const override;

    QHash<int, QByteArray> roleNames() const override;

    void loadTestData();
    Q_INVOKABLE void loadFolder(const QString &path);
    Q_INVOKABLE void loadFile(const QString& path);

    QStringList selectedFiles() const;

    void collectSelected(ProjectTreeItem* item, QStringList& out) const;

    ProjectContextData buildContext() const;

    void buildTreeText(
        ProjectTreeItem* item,
        QString& out,
        int depth) const;

    void collectSelectedFiles(
        ProjectTreeItem* item,
        QString& out) const;

    Q_INVOKABLE QString path() const;

    int selectedTokens() const;
    int collectTokens(
        ProjectTreeItem* item) const;

    int selectedFilesCount() const;
    qint64 selectedSize() const;

signals:
    void pathChanged();
    void selectedTokensChanged();
    void selectedFilesCountChanged();
    void selectedSizeChanged();

private:
    ProjectTreeItem* createItemTree(
        const ProjectNode& node,
        ProjectTreeItem* parent);
    void clearTree();
    void calculateTokens(ProjectTreeItem* item);
    int countSelectedFiles(ProjectTreeItem* item) const;
    qint64 collectSize(
        ProjectTreeItem* item) const;
    void calculateSizes(ProjectTreeItem* item);


private:
    ProjectTreeItem* m_root;
    ProjectContextService* m_service;
};

#endif // PROJECTTREEMODEL_H
