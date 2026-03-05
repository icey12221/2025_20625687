#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "partdialog.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QMenu>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>
#include <QTreeView>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // toolbar icon
    ui->actionopen->setIcon(QIcon(":/icons/icons/fileopen.png"));

    // ---- Build model tree ----
    partList = new ModelPartList("PartsList");
    ModelPart *rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("%1").arg(i + 1);
        ModelPart *childItem = new ModelPart({ name, "true" });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString childName = QString("%1.%2").arg(i + 1).arg(j + 1);
            ModelPart *childChildItem = new ModelPart({ childName, "true" });
            childItem->appendChild(childChildItem);
        }
    }

    // ---- Attach model to TreeView ----
    ui->treeView->setModel(partList);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);

    // ---- connections ----
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    connect(ui->ButtonA, &QPushButton::clicked,
            this, &MainWindow::handleButtonA);

    connect(ui->ButtonB, &QPushButton::clicked,
            this, &MainWindow::handleButtonB);

    connect(ui->actionopen, &QAction::triggered,
            this, &MainWindow::onOpenTriggered);

    // right click menu
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            this, &MainWindow::onTreeContextMenu);

    // optional existing action from ui
    if (ui->actionItem_Options) {
        connect(ui->actionItem_Options, &QAction::triggered,
                this, &MainWindow::on_actionItem_Options_triggered);
    }

    statusBar()->showMessage("Ready", 2000);

    // delay VTK init until widget has real size
    QTimer *vtkInitTimer = new QTimer(this);
    vtkInitTimer->setInterval(30);

    connect(vtkInitTimer, &QTimer::timeout, this, [this, vtkInitTimer]() {
        if (this->renderWindow) {
            vtkInitTimer->stop();
            vtkInitTimer->deleteLater();
            return;
        }

        if (ui->widget->width() > 10 && ui->widget->height() > 10) {
            vtkInitTimer->stop();
            vtkInitTimer->deleteLater();
            initVTK();
        }
    });

    vtkInitTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete partList;
}

void MainWindow::handleButtonA()
{
    statusBar()->showMessage("Button A clicked", 2000);
}

void MainWindow::handleButtonB()
{
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        statusBar()->showMessage("Please select a tree item first", 3000);
        return;
    }

    idx = idx.sibling(idx.row(), 0);

    auto *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part) {
        statusBar()->showMessage("Selection invalid", 3000);
        return;
    }

    PartDialog dlg(this);
    dlg.setFromModelPart(part);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.applyToModelPart(part);

        emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                                   {Qt::DisplayRole, Qt::EditRole});

        updateRender();
        statusBar()->showMessage("Dialog accepted", 3000);
    } else {
        statusBar()->showMessage("Dialog rejected", 3000);
    }
}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        statusBar()->showMessage("No valid selection", 2000);
        return;
    }

    auto *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        statusBar()->showMessage("Selection invalid", 2000);
        return;
    }

    statusBar()->showMessage("The selected item is: " +
                                 selectedPart->data(0).toString(), 3000);
}

void MainWindow::onOpenTriggered()
{
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        statusBar()->showMessage("Please select a tree item first", 3000);
        return;
    }

    idx = idx.sibling(idx.row(), 0);

    auto *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part) {
        statusBar()->showMessage("Selection invalid", 3000);
        return;
    }

    const QString path = QFileDialog::getOpenFileName(
        this,
        "Open STL File",
        "",
        "STL Files (*.stl);;All Files (*.*)"
        );

    if (path.isEmpty()) {
        statusBar()->showMessage("Open cancelled", 2000);
        return;
    }

    const QString filename = QFileInfo(path).fileName();

    // 1) rename current node
    part->set(0, filename);

    // 2) load stl into current node
    part->loadSTL(path);

    // 3) refresh tree display
    emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                               {Qt::DisplayRole, Qt::EditRole});

    // 4) refresh renderer
    updateRender();

    statusBar()->showMessage("Loaded STL into selected node: " + filename, 3000);
}

void MainWindow::onTreeContextMenu(const QPoint &pos)
{
    QModelIndex idx = ui->treeView->indexAt(pos);
    if (!idx.isValid())
        return;

    idx = idx.sibling(idx.row(), 0);
    ui->treeView->setCurrentIndex(idx);

    auto *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part)
        return;

    QMenu menu(this);
    QAction *editAct = menu.addAction("Edit properties...");
    QAction *addAct  = menu.addAction("Add part");

    QAction *chosen = menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    if (!chosen)
        return;

    if (chosen == editAct) {
        PartDialog dlg(this);
        dlg.setFromModelPart(part);

        if (dlg.exec() == QDialog::Accepted) {
            dlg.applyToModelPart(part);

            emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                                       {Qt::DisplayRole, Qt::EditRole});

            updateRender();
            statusBar()->showMessage("Item updated", 2000);
        }
    }
    else if (chosen == addAct) {
        QModelIndex newIdx = partList->appendChild(idx, { "New Part", "true" });

        if (newIdx.isValid()) {
            ui->treeView->expand(idx);
            ui->treeView->setCurrentIndex(newIdx);

            emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                                       {Qt::DisplayRole, Qt::EditRole});

            statusBar()->showMessage("New child part added", 2000);
        }
    }
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        statusBar()->showMessage("No tree item selected", 3000);
        return;
    }

    idx = idx.sibling(idx.row(), 0);

    auto *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part) {
        statusBar()->showMessage("Selection invalid", 3000);
        return;
    }

    PartDialog dlg(this);
    dlg.setFromModelPart(part);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.applyToModelPart(part);

        emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                                   {Qt::DisplayRole, Qt::EditRole});

        updateRender();
        statusBar()->showMessage("Item Options applied", 2000);
    } else {
        statusBar()->showMessage("Item Options cancelled", 2000);
    }
}

void MainWindow::initVTK()
{
    if (renderWindow)
        return;

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.08, 0.08, 0.08);
    renderWindow->AddRenderer(renderer);

    renderer->AutomaticLightCreationOn();
    renderer->TwoSidedLightingOn();

    renderer->ResetCamera();
    renderWindow->Render();
}

void MainWindow::updateRender()
{
    if (!renderer || !renderWindow || !partList)
        return;

    renderer->RemoveAllViewProps();

    const int topRows = partList->rowCount(QModelIndex());
    for (int i = 0; i < topRows; ++i) {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }

    renderer->ResetCamera();
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    auto *part = static_cast<ModelPart*>(index.internalPointer());
    if (part && part->visible()) {
        if (auto *a = part->getActor()) {
            renderer->AddActor(a);
        }
    }

    const int rows = partList->rowCount(index);
    for (int i = 0; i < rows; ++i) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
