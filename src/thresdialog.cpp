#include "thresdialog.h"
#include "ui_thresdialog.h"

#include <QRegularExpressionValidator>
#include <QRegularExpression>

ThresDialog::ThresDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThresDialog)
{
    ui->setupUi(this);
    ui->edit_input->setValidator(new QRegularExpressionValidator(QRegularExpression("^(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])$")));
}

ThresDialog::~ThresDialog()
{
    delete ui;
}

int ThresDialog::getThres() const
{
    QString inp = ui->edit_input->text();
    if (!inp.isEmpty()) {
        return std::stoi(inp.toStdString());
    }
    return -1;
}


void ThresDialog::on_buttonBox_accepted()
{
    QDialog::accept();
}


void ThresDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}

