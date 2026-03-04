#ifndef PARTDIALOG_H
#define PARTDIALOG_H

#include <QDialog>

class ModelPart;

namespace Ui {
class PartDialog;
}

class PartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PartDialog(QWidget *parent = nullptr);
    ~PartDialog();

    // 从选中项读值填进控件
    void setFromModelPart(ModelPart *part);

    // 从控件取值写回选中项
    void applyToModelPart(ModelPart *part);

private:
    Ui::PartDialog *ui;
};

#endif // PARTDIALOG_H
