#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QPoint>

#include "ModelPartList.h"
#include "ModelPart.h"

#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

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
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

private:
    Ui::MainWindow *ui = nullptr;
    ModelPartList *partList = nullptr;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    void initVTK();
};

#endif // MAINWINDOW_H
