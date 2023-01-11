#include "thicknessdialog.h"
#include "ui_thicknessdialog.h"
#include <QDialog>

ThicknessDialog::ThicknessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThicknessDialog)
{
    ui->setupUi(this);
    connect(ui->spinBox, &QSpinBox::valueChanged, ui->thickness_slider, &QSlider::setValue);
    connect(ui->thickness_slider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

ThicknessDialog::~ThicknessDialog()
{
    delete ui;
}

void ThicknessDialog::on_buttonBox_accepted()
{
    QDialog::accept();
}


void ThicknessDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}

int ThicknessDialog::getValue() const
{
    return ui->thickness_slider->value();
}

void ThicknessDialog::setValue(int value)
{
    ui->thickness_slider->setValue(value);
}
