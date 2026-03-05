#include "ModelPartList.h"
#include "ModelPart.h"

ModelPartList::ModelPartList(const QString& data, QObject* parent)
    : QAbstractItemModel(parent)
{
    Q_UNUSED(data);
    rootItem = new ModelPart({ tr("Part"), tr("Visible?") });
}

ModelPartList::~ModelPartList()
{
    delete rootItem;
    rootItem = nullptr;
}

int ModelPartList::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return rootItem ? rootItem->columnCount() : 0;
}

int ModelPartList::rowCount(const QModelIndex& parent) const
{
    if (!rootItem) return 0;
    if (parent.column() > 0) return 0;

    ModelPart* parentItem = nullptr;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    return parentItem ? parentItem->childCount() : 0;
}

QVariant ModelPartList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !rootItem)
        return QVariant();

    // TreeView 需要 DisplayRole（显示）和 EditRole（编辑后刷新）
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item)
        return QVariant();

    return item->data(index.column());
}

bool ModelPartList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item)
        return false;

    // ⚠️ 这里要求 ModelPart 里有 bool setData(int, const QVariant&)
    const bool changed = item->setData(index.column(), value);
    if (!changed)
        return false;

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

Qt::ItemFlags ModelPartList::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    // 允许选中 + 可编辑（否则 setData 很可能不会生效）
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ModelPartList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && rootItem)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const
{
    if (!rootItem || row < 0 || column < 0)
        return QModelIndex();

    ModelPart* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    if (!parentItem)
        return QModelIndex();

    ModelPart* childItem = parentItem->child(row);
    if (!childItem)
        return QModelIndex();

    return createIndex(row, column, childItem);
}

QModelIndex ModelPartList::parent(const QModelIndex& index) const
{
    if (!index.isValid() || !rootItem)
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    if (!childItem)
        return QModelIndex();

    ModelPart* parentItem = childItem->parentItem();
    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

ModelPart* ModelPartList::getRootItem()
{
    return rootItem;
}

QModelIndex ModelPartList::appendChild(QModelIndex& parent, const QList<QVariant>& data)
{
    ModelPart* parentPart = nullptr;

    if (parent.isValid())
        parentPart = static_cast<ModelPart*>(parent.internalPointer());
    else {
        parentPart = rootItem;
        parent = createIndex(0, 0, rootItem);
    }

    const int insertRow = rowCount(parent);
    beginInsertRows(parent, insertRow, insertRow);

    ModelPart* childPart = new ModelPart(data, parentPart);
    parentPart->appendChild(childPart);

    endInsertRows();

    // 返回新 child 的 index（注意 row 用 insertRow）
    return createIndex(insertRow, 0, childPart);
}
