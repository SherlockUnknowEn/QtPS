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
    void bitsLevels(const cv::Mat& src, std::vector<cv::Mat>& out);
    cv::Mat histogram_equalization(const cv::Mat& src);
    std::vector<float> calHist(const cv::Mat& src);
}
