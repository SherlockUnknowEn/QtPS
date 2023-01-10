#pragma once

#include <QWidget>
#include <QLabel>
#include <opencv2/opencv.hpp>

namespace Ui {
class Canvas;
}

class Canvas : public QLabel
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();
    void setMat(cv::Mat& img);
    cv::Mat getMat() const;
    void setScale(float scale);
    float getScale() const;
    void showMat();
    void setEditable(bool);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::Canvas *ui;
    cv::Mat m_img;
    float m_scale = 1.0;
    bool m_editable = false;
};

