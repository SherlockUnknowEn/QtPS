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
    enum class PaintType { Cursor, Line, Rect, Cicle, Eraser };
    ~Canvas();
    void setMat(cv::Mat& img);
    cv::Mat getMat() const;
    void showMat();

    void setScale(float scale);
    float getScale() const;

    void setEditable(bool flag);
    bool getEditable() const;

    void setPaintType(PaintType type);
    PaintType getPaintType() const;

    void setPaintColor(QColor color);
    QColor getPaintColor() const;

    void setPaintThickness(int sz);
    int getPaintThickness() const;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::Canvas *ui;
    cv::Mat m_img; // 原图
    cv::Mat m_editable_mat; // 可编辑图层，实际显示的图
    cv::Mat m_editable_last; // 画圆 矩形时，鼠标点击前的图像缓存
    cv::Mat m_mask; // 掩模图层, 橡皮擦功能需要使用，用掩模表示哪个部分需要从原图恢复

    float m_scale = 1.0;
    bool m_editable = false; // 是否可编辑
    bool m_is_press = false; // 鼠标是否按下
    QPoint m_mouse_pos; // 鼠标所在位置
    QPoint m_press_pos; // 鼠标按下的位置

    std::shared_ptr<QLabel> m_cursor_flare; // 编辑时鼠标光斑
    int m_paint_thickness = 10;
    PaintType m_paint_type = PaintType::Cursor;
    QColor m_paint_color = QColorConstants::Red;

    void updateMouseFlare();
    void maskAdd(cv::Mat& src, const cv::Mat& mask);

    void drawLine(const QPoint& begin, const QPoint& end);
    void drawCicle(const QPoint& tl, const QPoint& br);
    void drawRect(const QPoint& tl, const QPoint& br);
    void erase(const QPoint& begin, const QPoint& end);
};

