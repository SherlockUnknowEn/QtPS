#pragma once

#include <QWidget>
#include <QDialog>

namespace Ui {
class ThresDialog;
}

class ThresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThresDialog(QWidget *parent = nullptr);
    ~ThresDialog();
    int getThres() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ThresDialog *ui;
};

