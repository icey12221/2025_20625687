#include "partdialog.h"
#include "ui_partdialog.h"

#include "ModelPart.h"

PartDialog::PartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PartDialog)
{
    ui->setupUi(this);

    // SpinBox range
    ui->rSpin->setRange(0, 255);
    ui->gSpin->setRange(0, 255);
    ui->bSpin->setRange(0, 255);

    // OK/Cancel 已由 DialogButtonBox 自动连好 accept/reject
}

PartDialog::~PartDialog()
{
    delete ui;
}

void PartDialog::setFromModelPart(ModelPart *part)
{
    if (!part) return;

    // 列0：name
    ui->nameEdit->setText(part->data(0).toString());

    // 列1：visible（你的 demo 里是 "true"/"false" 字符串）
    const QString vis = part->data(1).toString().toLower();
    ui->visibleCheck->setChecked(vis == "true" || vis == "1" || vis == "yes");

    // 颜色：你 ModelPart 里还是 placeholder，先给默认值
    ui->rSpin->setValue(static_cast<int>(part->getColourR()));
    ui->gSpin->setValue(static_cast<int>(part->getColourG()));
    ui->bSpin->setValue(static_cast<int>(part->getColourB()));
}

void PartDialog::applyToModelPart(ModelPart *part)
{
    if (!part) return;

    // 用你现有的 ModelPart::set(int, QVariant)
    part->set(0, ui->nameEdit->text());
    part->set(1, ui->visibleCheck->isChecked() ? "true" : "false");

    part->setColour(
        static_cast<unsigned char>(ui->rSpin->value()),
        static_cast<unsigned char>(ui->gSpin->value()),
        static_cast<unsigned char>(ui->bSpin->value())
        );

    part->setVisible(ui->visibleCheck->isChecked());
}
