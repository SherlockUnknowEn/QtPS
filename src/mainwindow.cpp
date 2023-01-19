#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QMessageBox>
#include <QColorDialog>
#include <QColor>
#include <QtCharts/QtCharts>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>

#include "utils.h"
#include "my_dialog.h"

using namespace qtps;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/QTextCodec>
#else
    #include <QtCore5Compat/QTextCodec>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_toolBarActionGroup = std::make_shared<QActionGroup>(this);
    m_toolBarActionGroup->addAction(ui->action_cursor);
    m_toolBarActionGroup->addAction(ui->action_draw_line);
    m_toolBarActionGroup->addAction(ui->action_draw_cicle);
    m_toolBarActionGroup->addAction(ui->action_draw_rect);
    m_toolBarActionGroup->addAction(ui->action_eraser);

    this->m_button_group = {
        ui->btn_gray,
        ui->btn_gray_rev,
        ui->btn_binary,
        ui->btn_gamma,
        ui->btn_bit_levels,
        ui->btn_histogram_equalize,
        ui->btn_hist_mattch,
        ui->btn_mean_filter,
        ui->btn_median_filter,
        ui->btn_gauss_filter
    };
    this->enableButtons(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_gray_clicked()
{
    cv::Mat gray;
    cv::Mat img = ui->label_img1->getMat();
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    ui->label_img2->setMat(gray);
    ui->label_img2->showMat();
}


void MainWindow::on_btn_binary_clicked()
{
    std::shared_ptr<ThresDialog> dialog = std::make_shared<ThresDialog>(this);
    int code = dialog->exec();
    if (code == ThresDialog::DialogCode::Accepted) {
        cv::Mat bi;
        cv::Mat gray;
        cv::cvtColor(ui->label_img1->getMat(), gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, bi, dialog->getThres(), 255, cv::THRESH_BINARY);
        ui->label_img2->setMat(bi);
        ui->label_img2->showMat();
    }
}


void MainWindow::on_btn_gray_rev_clicked()
{

    cv::Mat img = ui->label_img1->getMat();
    cv::Mat gray = qtps::grayReverse(img);
    ui->label_img2->setMat(gray);
    ui->label_img2->showMat();
}

void MainWindow::on_btn_gamma_clicked()
{
    std::shared_ptr<GammaDialog> dialog = std::make_shared<GammaDialog>(this);
    int code = dialog->exec();
    if (code == GammaDialog::DialogCode::Accepted) {
        cv::Mat gamma = qtps::gammaTrans(ui->label_img1->getMat(), dialog->getAlpha(), dialog->getGamma());
        ui->label_img2->setMat(gamma);
        ui->label_img2->showMat();
    }
}

void MainWindow::enableButtons(bool flag) {
    for (auto& btn : m_button_group) {
        btn->setEnabled(flag);
    }
}

void MainWindow::on_btn_bit_levels_clicked()
{
    cv::Mat gray;
    cv::cvtColor(ui->label_img1->getMat(), gray, cv::COLOR_BGR2GRAY);
    ui->label_img2->setMat(gray);
    ui->label_img2->showMat();

    std::vector<cv::Mat> levels;
    for (size_t l = 0; l < 8; l++) {
        levels.push_back(cv::Mat(gray.size(), CV_8UC1));
    }

    qtps::bitsLevels(gray, levels);
    for (size_t l = 0; l < levels.size(); l++) {
        cv::imshow("Level " + std::to_string(l + 1), levels[l]);
    }
}

void MainWindow::on_btn_histogram_equalize_clicked()
{
    cv::Mat src = ui->label_img1->getMat();
    cv::Mat dst = qtps::histogram_equalization(src);
    std::vector<float> src_prob = qtps::calHist(src);
    std::vector<float> dst_prob = qtps::calHist(dst);
    showHistForm(src_prob, dst_prob);
    ui->label_img2->setMat(dst);
    ui->label_img2->showMat();
}

void MainWindow::showHistForm(const std::vector<float>& src_prob, const std::vector<float>& dst_prob)
{
    std::vector<QBarSet*> barsets;
    QBarSet *src_set = new QBarSet(tr("原图"));
    QBarSet *dst_set = new QBarSet(tr("均衡后"));
    //定义字符串列表，用于X轴标签
    QStringList categories;
    float max_val = 0.0;
    float src_val;
    float dst_val;
    for (size_t i = 1; i < src_prob.size(); i += 2) {
        categories << std::to_string(i).c_str();
        src_val = src_prob[i] - src_prob[i-1];
        dst_val = dst_prob[i] - dst_prob[i-1];
        *src_set << src_val;
        *dst_set << dst_val;
        max_val = std::max(max_val, src_val);
        max_val = std::max(max_val, dst_val);
    }

    src_set->setColor(QColorConstants::Red);
    dst_set->setColor(QColorConstants::Blue);

    QBarSeries *series = new QBarSeries();
    series->append(src_set);
    series->append(dst_set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("灰阶概率密度直方图");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    //设置X轴参数
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);//设置X标签
    chart->addAxis(axisX, Qt::AlignBottom); //将系列标签放到底下
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, max_val + 0.001);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    //设置标签对应是可视的
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
//    chart->setTheme(QChart::ChartThemeQt);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    m_hist_win = std::make_shared<QMainWindow>(this);
    m_hist_win->setCentralWidget(chartView);
    m_hist_win->resize(800, 600);
    m_hist_win->show();
}


void MainWindow::on_btn_hist_mattch_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开需要匹配的图像"), "",
                                                      tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    cv::Mat img = readFromFile(fileName);
    cv::Mat dst = qtps::histogram_mattch(ui->label_img1->getMat(), img);
    ui->label_img2->setMat(dst);
    ui->label_img2->showMat();
    cv::imshow(qtps::UTF82GBK("匹配的风格"), img);
}


cv::Mat MainWindow::readFromFile(QString fullpath)
{
    std::string gbk_filename = qtps::UTF82GBK(fullpath);
    cv::Mat img = cv::imread(gbk_filename, cv::IMREAD_COLOR);
    if (img.empty()) {
        QMessageBox::warning(this, tr("异常"), tr("加载图片出错"));
    }
    qDebug() << "fileName = " << QString(gbk_filename.c_str());
    qDebug() << img.cols << " " << img.rows;
    return img;
}

void MainWindow::on_action_open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开图像"), "",
                                                      tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    cv::Mat img = readFromFile(fileName);
    if (img.empty()) {
        return;
    }

    this->enableButtons(true);
    this->ui->label_img1->setMat(img);
    this->ui->label_img1->showMat();
}


void MainWindow::on_action_color_triggered()
{
    QColor i = QColorDialog::getColor(Qt::red, this,
                                              tr("颜色选择"),
                                              QColorDialog::ShowAlphaChannel);
    ui->label_img1->setPaintColor(i);
}


void MainWindow::on_action_cursor_triggered(bool checked)
{
    ui->label_img1->setEditable(!checked);
    ui->label_img1->setPaintType(Canvas::PaintType::Cursor);
}


void MainWindow::on_action_draw_line_triggered(bool checked)
{
    ui->label_img1->setEditable(checked);
    ui->label_img1->setPaintType(Canvas::PaintType::Line);
}


void MainWindow::on_action_draw_rect_triggered(bool checked)
{
    ui->label_img1->setEditable(checked);
    ui->label_img1->setPaintType(Canvas::PaintType::Rect);
}


void MainWindow::on_action_draw_cicle_triggered(bool checked)
{
    ui->label_img1->setEditable(checked);
    ui->label_img1->setPaintType(Canvas::PaintType::Cicle);
}


void MainWindow::on_action_eraser_triggered(bool checked)
{
    ui->label_img1->setEditable(checked);
    ui->label_img1->setPaintType(Canvas::PaintType::Eraser);
}


void MainWindow::on_action_thickness_triggered()
{
    std::shared_ptr<NumInputDialog> thickness_dialog = std::make_shared<NumInputDialog>(this);
    thickness_dialog->setDialogAttr("线条粗细", 1, 50, 10);
    thickness_dialog->setValue(ui->label_img1->getPaintThickness());
    int code = thickness_dialog->exec();
    if (code == NumInputDialog::Accepted) {
        ui->label_img1->setPaintThickness(thickness_dialog->getValue());
    }
}


void MainWindow::on_btn_mean_filter_clicked()
{
    std::shared_ptr<NumInputDialog> filter_dialog = std::make_shared<NumInputDialog>(this);
    filter_dialog->setDialogAttr("滤波器大小", 3, 50, 5);
    int code = filter_dialog->exec();
    if (code == NumInputDialog::Accepted) {
        int sz = filter_dialog->getValue();
        cv::Mat dst;
        cv::blur(this->ui->label_img1->getMat(), dst, cv::Size(sz, sz));
        this->ui->label_img2->setMat(dst);
        this->ui->label_img2->showMat();
    }
}


void MainWindow::on_btn_median_filter_clicked()
{
    std::shared_ptr<NumInputDialog> filter_dialog = std::make_shared<NumInputDialog>(this);
    filter_dialog->setDialogAttr("滤波器大小", 3, 50, 5);
    int code = filter_dialog->exec();
    if (code == NumInputDialog::Accepted) {
        int sz = filter_dialog->getValue();
        if (sz % 2 == 0) {
            QMessageBox::critical(this, "警告", "滤波器大小必须为奇数");
            return;
        }
        cv::Mat dst;
        cv::medianBlur(this->ui->label_img1->getMat(), dst, sz);
        this->ui->label_img2->setMat(dst);
        this->ui->label_img2->showMat();
    }
}


void MainWindow::on_btn_gauss_filter_clicked()
{
    std::shared_ptr<NumInputDialog> filter_dialog = std::make_shared<NumInputDialog>(this);
    filter_dialog->setDialogAttr("滤波器大小", 3, 50, 5);
    int code = filter_dialog->exec();
    if (code == NumInputDialog::Accepted) {
        int sz = filter_dialog->getValue();
        if (sz % 2 == 0) {
            QMessageBox::critical(this, "警告", "滤波器带下必须为奇数");
            return;
        }
        cv::Mat dst;
        cv::GaussianBlur(this->ui->label_img1->getMat(), dst, cv::Size(sz, sz), 2);
        this->ui->label_img2->setMat(dst);
        this->ui->label_img2->showMat();
    }
}


void MainWindow::on_btn_laplacian_clicked()
{

}

