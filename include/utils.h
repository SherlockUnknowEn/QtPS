#pragma once

#include <string.h>
#include <QString>
#include <opencv2/opencv.hpp>
#include <QImage>

namespace qtps {
    std::string UTF82GBK(const QString& s);
    QImage CvMat2QImage(const cv::Mat &mat);
    cv::Mat QImage2CvMat(const QImage &image);

    cv::Mat grayReverse(const cv::Mat& src);
    cv::Mat gammaTrans(const cv::Mat& src, float alpha, float gamma);
    void bitsLevels(const cv::Mat& src, std::vector<cv::Mat>& out); // 比特平面分层
    std::vector<float> calHist(const cv::Mat& src); // 计算灰度概率密度
    cv::Mat histogram_equalization(const cv::Mat& src); // 直方图均衡
    cv::Mat histogram_mattch(const cv::Mat& from, const cv::Mat& to); // 直方图匹配

    template<class... T>
    std::string format(const char *fmt, const T&...t);
}
