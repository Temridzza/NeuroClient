#ifndef PROJECTTREEMODEL_H
#define PROJECTTREEMODEL_H

#pragma once

#include <QAbstractItemModel>
#include "ProjectTreeItem.h"

class ProjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles
    {
        NameRole = Qt::UserRole + 1,
        PathRole,
        TypeRole,
        SelectedRole
    };

    explicit ProjectTreeModel(ProjectTreeItem* root, QObject* parent = nullptr);

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

private:
    ProjectTreeItem* m_root;
};

#endif // PROJECTTREEMODEL_H
