#pragma once

#include <QWidget>
#include <QDialog>

namespace Ui {
class ThicknessDialog;
}

class ThicknessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThicknessDialog(QWidget *parent = nullptr);
    ~ThicknessDialog();
    int getValue() const;
    void setValue(int value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ThicknessDialog *ui;
};

