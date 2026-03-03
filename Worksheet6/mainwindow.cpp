#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "partdialog.h"

#include <QPushButton>
#include <QStatusBar>
#include <QTreeView>
#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QModelIndex>
#include <QIcon>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Toolbar/Menu icon
    ui->actionopen->setIcon(QIcon(":/icons/icons/fileopen.png"));

    // ---- Build model tree ----
    partList = new ModelPartList("PartsList");
    ModelPart *rootItem = partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("%1").arg(i + 1);   // 1,2,3
        QString visible("true");

        ModelPart *childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString childName = QString("%1.%2").arg(i + 1).arg(j + 1); // 1.1...3.5
            QString childVisible("true");

            ModelPart *childChildItem = new ModelPart({ childName, childVisible });
            childItem->appendChild(childChildItem);
        }
    }

    // ---- Attach model to TreeView ----
    ui->treeView->setModel(partList);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);

    ui->treeView->addAction(ui->actionItem_Options);

    // ---- Connections ----
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    connect(ui->ButtonA, &QPushButton::clicked,
            this, &MainWindow::handleButtonA);

    connect(ui->ButtonB, &QPushButton::clicked,
            this, &MainWindow::handleButtonB);

    connect(ui->actionopen, &QAction::triggered,
            this, &MainWindow::onOpenTriggered);

    // ✅ Right-click context menu (IMPORTANT)
    //ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui->treeView, &QTreeView::customContextMenuRequested,
            //this, &MainWindow::onTreeContextMenu);

    statusBar()->showMessage("Ready", 2000);
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
    // 取当前选中 item（如果没选就提示）
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

        // 刷新两列
        emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                                   {Qt::DisplayRole, Qt::EditRole});

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

    const QString text = selectedPart->data(0).toString();
    statusBar()->showMessage("The selected item is: " + text, 3000);
}

void MainWindow::onOpenTriggered()
{
    QString startDir;
    if (ui->input) {
        startDir = ui->input->toPlainText().trimmed();
    }

    const QString path = QFileDialog::getOpenFileName(
        this,
        "Open File",
        startDir,
        "All Files (*.*)"
        );

    if (path.isEmpty()) {
        statusBar()->showMessage("Open cancelled", 2000);
        return;
    }

    if (ui->input) {
        ui->input->setPlainText(path);
    }

    const QString filename = QFileInfo(path).fileName();

    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        statusBar()->showMessage("Please select a tree item first", 3000);
        return;
    }

    idx = idx.sibling(idx.row(), 0);

    const bool ok = partList->setData(idx, filename, Qt::EditRole);
    statusBar()->showMessage(ok ? ("Renamed selected item to: " + filename)
                                : "Rename failed (setData returned false)", 3000);
}

void MainWindow::onTreeContextMenu(const QPoint &pos)
{
    QModelIndex idx = ui->treeView->indexAt(pos);
    if (!idx.isValid())
        return;

    idx = idx.sibling(idx.row(), 0);

    auto *part = static_cast<ModelPart*>(idx.internalPointer());
    if (!part)
        return;

    QMenu menu(this);
    QAction *editAct = menu.addAction("Edit properties...");
    QAction *chosen = menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    if (chosen != editAct)
        return;

    PartDialog dlg(this);
    dlg.setFromModelPart(part);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.applyToModelPart(part);

        // Refresh both columns
        emit partList->dataChanged(idx, idx.sibling(idx.row(), 1),
                                   {Qt::DisplayRole, Qt::EditRole});

        statusBar()->showMessage("Item updated", 2000);
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

        statusBar()->showMessage("Item Options applied", 2000);
    } else {
        statusBar()->showMessage("Item Options cancelled", 2000);
    }
}
