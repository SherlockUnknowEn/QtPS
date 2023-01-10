#pragma once

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <qpushbutton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<QPushButton*> m_buttons;
    std::shared_ptr<QMainWindow> m_hist_win; // 直方图的图表窗口

    void enableButtons(bool flag);
    void showHistForm(const std::vector<float>& prob, const std::vector<float>& dst_prob);

private slots:
    void on_open_action_clicked(); // 菜单点击

    void on_btn_gray_clicked();
    void on_btn_binary_clicked();
    void on_btn_gray_rev_clicked();
    void on_btn_gamma_clicked();
    void on_btn_bit_levels_clicked();
    void on_btn_histogram_equalize_clicked();
    void on_btn_hist_mattch_clicked();
};
