#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

#include "ModelPartList.h"
#include "ModelPart.h"
#include <QPoint>

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
    void onOpenTriggered();   // ✅和 cpp 对齐
    void onTreeContextMenu(const QPoint &pos);
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList *partList = nullptr;
};

#endif // MAINWINDOW_H
