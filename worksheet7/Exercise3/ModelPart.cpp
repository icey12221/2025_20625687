/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"

//#include <vtkSmartPointer.h>
//#include <vtkDataSetMapper.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data)
    , m_parentItem(parent)
{
    // Default values
    m_r = 0;
    m_g = 0;
    m_b = 0;
    m_visible = true;

    // If column 1 exists, keep it consistent with default visible
    if (m_itemData.size() > 1) {
        m_itemData.replace(1, QString("true"));
    }
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item)
{
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const
{
    return m_childItems.count();
}

int ModelPart::columnCount() const
{
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size())
        return;
    m_itemData.replace(column, value);
}

bool ModelPart::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size())
        return false;

    if (m_itemData[column] == value)
        return false;

    m_itemData.replace(column, value);
    return true;
}

ModelPart* ModelPart::parentItem()
{
    return m_parentItem;
}

int ModelPart::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

// --------- Colour / Visible: REAL IMPLEMENTATION (so dialog values persist) ---------

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B)
{
    m_r = R;
    m_g = G;
    m_b = B;
}

unsigned char ModelPart::getColourR()
{
    return m_r;
}

unsigned char ModelPart::getColourG()
{
    return m_g;
}

unsigned char ModelPart::getColourB()
{
    return m_b;
}

void ModelPart::setVisible(bool isVisible)
{
    m_visible = isVisible;

    // Keep column 1 ("Visible?") consistent for the TreeView display if it exists
    if (m_itemData.size() > 1) {
        m_itemData.replace(1, m_visible ? QString("true") : QString("false"));
    }
}

bool ModelPart::visible()
{
    return m_visible;
}

// --------- VTK placeholders (leave as-is) ---------

void ModelPart::loadSTL(QString fileName)
{
    Q_UNUSED(fileName);
    // Placeholder for later VTK work
}
