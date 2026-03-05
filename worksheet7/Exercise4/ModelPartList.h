/**     @file ModelPartList.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model part list that will be used to create the treeview.
  *
  *     P Evans 2022
  */

#ifndef VIEWER_MODELPARTLIST_H
#define VIEWER_MODELPARTLIST_H

#include "ModelPart.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QList>

class ModelPart;

class ModelPartList : public QAbstractItemModel {
    Q_OBJECT
public:
    ModelPartList(const QString& data, QObject* parent = nullptr);
    ~ModelPartList() override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    ModelPart* getRootItem();

    QModelIndex appendChild(QModelIndex& parent, const QList<QVariant>& data);

private:
    ModelPart *rootItem = nullptr;
};

#endif // VIEWER_MODELPARTLIST_H
