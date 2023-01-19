#include "numinputdialog.h"
#include "ui_numinputdialog.h"
#include <QDialog>

NumInputDialog::NumInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumInputDialog)
{
    ui->setupUi(this);
    connect(ui->spinBox, &QSpinBox::valueChanged, ui->slider, &QSlider::setValue);
    connect(ui->slider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

NumInputDialog::~NumInputDialog()
{
    delete ui;
}

void NumInputDialog::on_buttonBox_accepted()
{
    QDialog::accept();
}


void NumInputDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}

int NumInputDialog::getValue() const
{
    return ui->slider->value();
}

void NumInputDialog::setValue(int value)
{
    ui->slider->setValue(value);
}

void NumInputDialog::setText(const std::string& text)
{
    this->ui->label->setText(QString::fromStdString(text));
}

void NumInputDialog::setMax(int m)
{
    this->ui->slider->setMaximum(m);
    this->ui->spinBox->setMaximum(m);
}

void NumInputDialog::setMin(int m)
{
    this->ui->slider->setMinimum(m);
    this->ui->spinBox->setMinimum(m);
}

void NumInputDialog::setDialogAttr(const std::string& text, int min, int max, int value)
{
    this->setText(text);
    this->setMin(min);
    this->setMax(max);
    this->setValue(value);
}
