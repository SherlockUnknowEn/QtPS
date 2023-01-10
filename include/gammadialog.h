#pragma once

#include <QWidget>
#include <QDialog>

namespace Ui {
class GammaDialog;
}

class GammaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GammaDialog(QWidget *parent = nullptr);
    ~GammaDialog();
    float getGamma() const;
    float getAlpha() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::GammaDialog *ui;
};

