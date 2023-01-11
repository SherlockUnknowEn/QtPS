#include "canvas.h"
#include "ui_canvas.h"
#include <QWheelEvent>
#include <QString>
#include <sstream>
#include "utils.h"
#include <stdio.h>

Canvas::Canvas(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::Canvas)
{
    ui->setupUi(this);
    m_cursor_flare = std::make_shared<QLabel>(this);
}

Canvas::~Canvas()
{
    delete ui;
}

// 重写滚轮事件
void Canvas::wheelEvent(QWheelEvent *event)
{
    // 未加载图片
    if (m_img.empty()) {
        return;
    }
    // 已经点击，正在编辑图片
    if (m_is_press) {
        return;
    }
    QPoint delta = event->angleDelta();
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

// 重写鼠标点击事件
void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << event->pos();
        qDebug() << this->size();
        this->m_mouse_pos = event->pos();
        this->m_press_pos = event->pos();
        this->m_is_press = true;
        // 如果绘画矩形和圆形，先缓存当前图像
        if (m_paint_type == PaintType::Cursor) {

        } else if ( m_paint_type == PaintType::Line) {

        } else if ( m_paint_type == PaintType::Eraser) {
            this->m_mask = cv::Mat(m_editable_mat.size(), CV_8UC3);
        } else if (m_paint_type == PaintType::Rect) {
            this->m_editable_last = this->m_editable_mat.clone();
        } else if (m_paint_type == PaintType::Cicle) {
            this->m_editable_last = this->m_editable_mat.clone();
        }
    }
}

// 重写鼠标移动事件
// setMouseTracking 默认为false，只有点击等鼠标事件触发时，才跟踪鼠标坐标
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_editable) return;

    QPoint pos = event->pos();
    // 编辑状态下，设置鼠标有圆圈跟随，提示线条粗细
    if (m_paint_type == PaintType::Line ||  m_paint_type == PaintType::Eraser) {
        m_cursor_flare->move(pos.x() - m_cursor_flare->width() / 2, pos.y() - m_cursor_flare->height() / 2);
    }

    if (pos.x() > this->size().width() || pos.y() > this->size().height()) {
        return;
    }

    // 触发画线
    if (m_is_press && m_paint_type == PaintType::Line) {
        drawLine(m_mouse_pos, pos);
    }
    // 触发橡皮擦
    else if (m_is_press && m_paint_type == PaintType::Eraser) {
        erase(m_mouse_pos, pos);
    }
    // 触发画框
    else if (m_is_press && m_paint_type == PaintType::Rect) {
        drawRect(m_press_pos, pos);
    }
    // 触发画圆
    else if (m_is_press && m_paint_type == PaintType::Cicle) {
        drawCicle(m_press_pos, pos);
    }
    this->showMat();
    m_mouse_pos = pos;
}

// 重写鼠标松开事件
void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->m_is_press = false;
    }
}


void Canvas::updateMouseFlare()
{
    m_cursor_flare->resize(m_paint_thickness, m_paint_thickness);
    //设置鼠标穿透
    m_cursor_flare->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    std::stringstream oss;
    oss << "border-width: 2px; border-style: solid; border-color:rgb(" << m_paint_color.red() << "," << m_paint_color.green() << "," << m_paint_color.blue()
        << "); border-radius: " << m_paint_thickness / 2 << "px;";
//    std::string style = qtps::format("background-color:rgba(%d, %d, %d, %.2f);  border-radius: %d px;",
//                                     m_paint_color.red(),  m_paint_color.green(), m_paint_color.blue(), 0.3, m_paint_thickness / 2);
    m_cursor_flare->setStyleSheet(QString(oss.str().c_str()));

    // setMouseTracking 默认为false，只有点击等鼠标事件触发时，才跟踪鼠标坐标
    // 设置鼠标光斑
    if (this->m_paint_type == PaintType::Line || this->m_paint_type == PaintType::Eraser) {
        // 打开鼠标跟踪事件
        this->setMouseTracking(true);
        m_cursor_flare->show();
    } else {
        this->setMouseTracking(false);
        m_cursor_flare->hide();
    }
}


void Canvas::drawLine(const QPoint& begin, const QPoint& end)
{
    cv::Scalar color = cv::Scalar(m_paint_color.blue(), m_paint_color.green(), m_paint_color.red(), m_paint_color.alpha());
    cv::line(this->m_editable_mat, cv::Point(begin.x(), begin.y()), cv::Point(end.x(), end.y()), color, m_paint_thickness);
}


void Canvas::drawRect(const QPoint& tl, const QPoint& br)
{
    cv::Scalar color = cv::Scalar(m_paint_color.blue(), m_paint_color.green(), m_paint_color.red(), m_paint_color.alpha());
    this->m_editable_mat = this->m_editable_last.clone();
    int x1 = std::min(tl.x(), br.x());
    int y1 = std::min(tl.y(), br.y());
    int x2 = std::max(tl.x(), br.x());
    int y2 = std::max(tl.y(), br.y());
    cv::rectangle(this->m_editable_mat, cv::Point(x1, y1), cv::Point(x2, y2), color, m_paint_thickness);
}

void Canvas::drawCicle(const QPoint& tl, const QPoint& br)
{
    cv::Scalar color = cv::Scalar(m_paint_color.blue(), m_paint_color.green(), m_paint_color.red(), m_paint_color.alpha());
    this->m_editable_mat = this->m_editable_last.clone();
    int x1 = std::min(tl.x(), br.x());
    int y1 = std::min(tl.y(), br.y());
    int x2 = std::max(tl.x(), br.x());
    int y2 = std::max(tl.y(), br.y());

    int x = (x1 + x2) / 2;
    int y = (y1 + y2) / 2;
    int w = (x2 - x1) / 2;
    int h = (y2 - y1) / 2;
    cv::ellipse(this->m_editable_mat, cv::Point(x, y), cv::Size(w, h), 0, 0, 360,
                    color, m_paint_thickness);
}

void Canvas::erase(const QPoint& begin, const QPoint& end)
{
    cv::line(this->m_mask, cv::Point(begin.x(), begin.y()),
             cv::Point(end.x(), end.y()), cv::Scalar(255, 255, 255), m_paint_thickness);
    cv::Mat tmp;
    cv::resize(m_img, tmp, m_editable_mat.size());
    for (size_t i = 0; i < m_editable_mat.rows; i++) {
        for (size_t j = 0; j < m_editable_mat.cols; j++) {
            if (this->m_mask.at<cv::Vec3b>(i, j)[0] == 255) {
                m_editable_mat.at<cv::Vec3b>(i, j) = tmp.at<cv::Vec3b>(i, j);
            }
        }
    }
}

void Canvas::setMat(cv::Mat& img)
{
    this->m_img = img;
    this->m_editable_mat = img.clone();
    this->setScale(m_scale);
}

cv::Mat Canvas::getMat() const
{
    return m_img;
}


void Canvas::showMat()
{
    if (m_img.empty()) {
        return;
    }

    QImage pixmap = qtps::CvMat2QImage(m_editable_mat);
//    QImage pixmap = qtps::CvMat2QImage(m_img);
//    this->setPixmap(QPixmap::fromImage(pixmap).scaled(this->size(), Qt::KeepAspectRatio));
    this->setPixmap(QPixmap::fromImage(pixmap));
//    this->setScaledContents(true);
}


void Canvas::setScale(float scale)
{
    this->m_scale = scale;
    float w = m_img.cols * this->m_scale;
    float h = m_img.rows * this->m_scale;
    this->resize((int)w, (int)h);
    // 绘画图层也需要跟着缩放
    cv::resize(m_editable_mat, m_editable_mat, cv::Size((int)w, (int)h)); // BGRA
}

float Canvas::getScale() const
{
    return this->m_scale;
}

void Canvas::setEditable(bool flag)
{
    this->m_editable = flag;
}

bool Canvas::getEditable() const
{
    return this->m_editable;
}


void Canvas::setPaintType(PaintType type)
{
    // 设置鼠标样式
    if (type == PaintType::Cursor) {
        this->setCursor(Qt::ArrowCursor);
    } else {
        this->setCursor(Qt::CrossCursor);
    }
    this->m_paint_type = type;
    this->updateMouseFlare();
}

Canvas::PaintType Canvas::getPaintType() const
{
    return this->m_paint_type;
}

void Canvas::setPaintColor(QColor color)
{
    this->m_paint_color = color;
    this->updateMouseFlare();
}

QColor Canvas::getPaintColor() const
{
    return this->m_paint_color;
}


void Canvas::setPaintThickness(int sz)
{
    this->m_paint_thickness = sz;
    this->updateMouseFlare();
}

int Canvas::getPaintThickness() const
{
    return this->m_paint_thickness;
}
