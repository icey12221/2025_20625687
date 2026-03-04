#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QPoint>

#include "ModelPartList.h"
#include "ModelPart.h"

#include <QShowEvent>

// ---- VTK forward declarations + smart pointer ----
#include <vtkSmartPointer.h>
class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

class vtkCylinderSource;
class vtkPolyDataMapper;
class vtkActor;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButtonA();
    void handleButtonB();
    void handleTreeClicked(const QModelIndex &index);
    void onOpenTriggered();
    void onTreeContextMenu(const QPoint &pos);
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList *partList = nullptr;

    // ---- VTK members for Exercise 3 ----
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    void initVTK();
    vtkSmartPointer<vtkCylinderSource> cylinder;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> cylinderActor;
};


#endif // MAINWINDOW_H
