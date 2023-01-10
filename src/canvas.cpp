#include "canvas.h"
#include "ui_canvas.h"
#include <QWheelEvent>
#include "utils.h"

Canvas::Canvas(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::Canvas)
{
    ui->setupUi(this);
}

Canvas::~Canvas()
{
    delete ui;
}

// 重写滚轮事件
void Canvas::wheelEvent(QWheelEvent *event)
{
    if (m_img.empty()) {
        return;
    }
    QPoint delta = event->angleDelta();
    qDebug() << delta.rx() << " " << delta.ry();
    if (delta.ry() > 0) {
        // 放大
        if (m_scale >= 5.0) return;
        this->setScale(m_scale + 0.2);
    } else {
        if (m_scale <= 0.5) return;
        this->setScale(m_scale - 0.2);
    }
    this->showMat();
}


void Canvas::setMat(cv::Mat& img)
{
    this->m_img = img;
    this->setScale(m_scale);
}

cv::Mat Canvas::getMat() const
{
    return m_img;
}

void Canvas::setScale(float scale)
{
    this->m_scale = scale;
    float w = m_img.cols * this->m_scale;
    float h = m_img.rows * this->m_scale;
    this->resize((int)w, (int)h);
}

float Canvas::getScale() const
{
    return this->m_scale;
}

void Canvas::showMat()
{
    if (m_img.empty()) {
        return;
    }
    QImage pixmap = qtps::CvMat2QImage(m_img);
//    this->setPixmap(QPixmap::fromImage(pixmap));
    this->setPixmap(QPixmap::fromImage(pixmap).scaled(this->size(), Qt::KeepAspectRatio));
//    this->setScaledContents(true);
}

void Canvas::setEditable(bool flag)
{
    this->m_editable = flag;
}
