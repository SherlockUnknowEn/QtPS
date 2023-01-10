#include "gammadialog.h"
#include "ui_gammadialog.h"
#include <QRegularExpressionValidator>
#include <QRegularExpression>

GammaDialog::GammaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GammaDialog)
{
    ui->setupUi(this);
    ui->edit_alpha->setValidator(new QRegularExpressionValidator(QRegularExpression("(^[0]|^[1-9][0-9]*)([.]{1}[0-9]*){0,1}$"))); // 匹配正实数
    ui->edit_gamma->setValidator(new QRegularExpressionValidator(QRegularExpression("(^[0]|^[1-9][0-9]*)([.]{1}[0-9]*){0,1}$"))); // 匹配正实数
}

GammaDialog::~GammaDialog()
{
    delete ui;
}

void GammaDialog::on_buttonBox_accepted()
{
    QDialog::accept();
}


void GammaDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}

float GammaDialog::getGamma() const
{
    return ui->edit_gamma->text().toFloat();
}

float GammaDialog::getAlpha() const
{
    return ui->edit_alpha->text().toFloat();
}
