/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

#include <vtkSmartPointer.h>
#include <vtkActor.h>

class vtkSTLReader;
class vtkPolyDataMapper;

class ModelPart
{
public:
    /** Return actor for GUI rendering */
    vtkActor* getActor() const;

    bool setData(int column, const QVariant &value);

    /** Constructor
     * @param data is a List (array) of strings for each property of this item
     * @param parent is the parent of this item
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor */
    ~ModelPart();

    /** Add a child to this item */
    void appendChild(ModelPart* item);

    /** Return child at position row */
    ModelPart* child(int row);

    /** Return number of children */
    int childCount() const;

    /** Return number of visible data columns */
    int columnCount() const;

    /** Return the data item at a particular column */
    QVariant data(int column) const;

    /** Set a property */
    void set(int column, const QVariant& value);

    /** Get pointer to parent item */
    ModelPart* parentItem();

    /** Get row index of item, relative to parent item */
    int row() const;

    /** Set colour */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    /** Set visible flag */
    void setVisible(bool isVisible);

    /** Get visible flag */
    bool visible();

    /** Load STL file */
    void loadSTL(QString fileName);

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant> m_itemData;
    ModelPart* m_parentItem = nullptr;

    // VTK objects for this part
    vtkSmartPointer<vtkSTLReader> stlReader;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    // Part properties
    unsigned char m_r = 0;
    unsigned char m_g = 0;
    unsigned char m_b = 0;
    bool m_visible = true;
};

#endif // VIEWER_MODELPART_H
