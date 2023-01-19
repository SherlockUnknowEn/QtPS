#pragma once

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <qpushbutton>
#include <QActionGroup>

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
    std::shared_ptr<QActionGroup> m_toolBarActionGroup;
    std::vector<QPushButton*> m_button_group;
    std::shared_ptr<QMainWindow> m_hist_win; // 直方图的图表窗口

    void enableButtons(bool flag);
    void showHistForm(const std::vector<float>& prob, const std::vector<float>& dst_prob);
    cv::Mat readFromFile(QString fullpath);

private slots:

    void on_btn_gray_clicked();
    void on_btn_binary_clicked();
    void on_btn_gray_rev_clicked();
    void on_btn_gamma_clicked();
    void on_btn_bit_levels_clicked();
    void on_btn_histogram_equalize_clicked();
    void on_btn_hist_mattch_clicked();
    void on_action_color_triggered();
    void on_action_open_triggered();
    void on_action_cursor_triggered(bool checked);
    void on_action_draw_line_triggered(bool checked);
    void on_action_draw_rect_triggered(bool checked);
    void on_action_eraser_triggered(bool checked);
    void on_action_draw_cicle_triggered(bool checked);
    void on_action_thickness_triggered();
    void on_btn_mean_filter_clicked();
    void on_btn_median_filter_clicked();
    void on_btn_gauss_filter_clicked();
    void on_btn_laplacian_clicked();
};
