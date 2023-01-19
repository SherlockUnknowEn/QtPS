#pragma once

#include <QWidget>
#include <QDialog>

namespace Ui {
class NumInputDialog;
}

class NumInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NumInputDialog(QWidget *parent = nullptr);
    ~NumInputDialog();
    int getValue() const;
    void setValue(int value);

    void setText(const std::string& text);
    void setMax(int m);
    void setMin(int m);
    void setDialogAttr(const std::string& text, int min, int max, int value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::NumInputDialog *ui;
};

