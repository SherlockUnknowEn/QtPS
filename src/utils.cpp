#include "utils.h"
#include <unordered_map>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/QTextCodec>
#else
    #include <QtCore5Compat/QTextCodec>
#endif

namespace qtps {

    // 字符串格式化
    template<class... T>
    std::string format(const char *fmt, const T&...t)
    {
        const auto len = snprintf(nullptr, 0, fmt, t...);
        std::string r;
        r.resize(static_cast<size_t>(len) + 1);
        snprintf(&r.front(), len + 1, fmt, t...);  // Bad boy
        r.resize(static_cast<size_t>(len));

        return r;
    }

    std::string UTF82GBK(const QString& s) {
        QTextCodec *code = QTextCodec::codecForName("GBK");//解决中文路径问题
        std::string gbkstr = code->fromUnicode(s).data();
        return gbkstr;
    }

    QImage CvMat2QImage(const cv::Mat &mat)
    {
        // 图像的通道
        int channel = mat.channels();

        // 设立一个表 直接查询 其中 0 2 是无效值 1 3 4 对应的转换值
        const std::unordered_map<int, QImage::Format> img_cvt_map = {
            { 1, QImage::Format_Grayscale8 },
            { 3, QImage::Format_RGB888 },
            { 4, QImage::Format_ARGB32 }
        };

        QImage image(mat.data, mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     img_cvt_map.at(channel));

        // 三通道图像 值做 bgr2rgb通道转换
        return channel == 3 ? image.rgbSwapped() : image;
    }

    cv::Mat QImage2CvMat(const QImage &image)
    {
        cv::Mat mat;
        const std::map<QImage::Format, int> img_cvt_map {
            { QImage::Format_Grayscale8, 1 },
            { QImage::Format_RGB888, 3 },
            { QImage::Format_ARGB32, 4}
        };

        return cv::Mat(image.height(), image.width(), img_cvt_map.at(image.format()));
    }

    /**
     * 灰度反转
     * @brief grayReverse
     * @param src
     * @return
     */
    cv::Mat grayReverse(const cv::Mat& src)
    {
        cv::Mat gray;
        if (src.channels() == 3) {
            cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        }
        if (src.channels() == 1) {
            gray = src;
        }
        for (int i = 0; i < gray.rows; i++) {
            for (int j = 0; j < gray.cols; j++) {
                gray.at<uchar>(i, j) = 255 - gray.at<uchar>(i, j);
            }
        }
        return gray;
    }

    /**
     * 伽马变换
     * @brief gammaTrans
     * @param src 原图像
     * @param alpha 偏移系数
     * @param gamma 幂系数
     * @return 变换后图像
     */
    cv::Mat gammaTrans(const cv::Mat& src, float alpha, float gamma)
    {
        int channel = src.channels();
        cv::Mat dst(src.size(), channel == 3 ? CV_32FC3 : CV_32FC1);

        for (int i = 0; i < dst.rows; i++) {
            for (int j = 0; j < dst.cols; j++) {
                for (int c = 0; c < channel; c++) {
                    if (channel == 3) {
                        dst.at<cv::Vec3f>(i, j)[c] = alpha * std::pow(src.at<cv::Vec3b>(i, j)[c], gamma);
                    } else if (channel == 1) {
                        dst.at<float>(i, j) = alpha * std::pow(src.at<uchar>(i, j), gamma);
                    }
                }
            }
        }
        cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    //    cv::convertScaleAbs(dst, dst); // 转成8bit
        dst.convertTo(dst, CV_8UC3); // 转成8bit
        return dst;
    }


    /**
     * 比特平面分层
     * @brief bitsLevels
     * @param src
     * @param out
     */
    void bitsLevels(const cv::Mat& src, std::vector<cv::Mat>& out)
    {
        // std::vector<uchar> thres = { 1, 2, 4, 8, 16, 32, 64, 128 };
        cv::Mat gray;
        if (src.channels() == 3) {
            cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        }
        if (src.channels() == 1) {
            gray = src;
        }
        for (size_t i = 0; i < out.size(); i++) {
            cv::threshold(gray, out[i], 0x01<<i, 255, cv::THRESH_BINARY);
        }
    }

    /**
     * 直方图均衡
     * @brief histogram_equalization
     * @param src
     * @return
     */
    cv::Mat histogram_equalization(const cv::Mat& src) {
        cv::Mat gray;
        if (src.channels() == 3) {
            cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        }
        if (src.channels() == 1) {
            gray = src;
        }
        // 手写 直方图均衡
        // 1. 计算原始灰阶的概率密度
//        std::vector<float> prob = qtps::calHist(gray);
//        // 2. 根据概率密度，重新分配灰阶 s = 255 * Σ(prob(r))
//        for (size_t i = 0; i < gray.rows; i++) {
//            for (size_t j = 0; j < gray.cols; j++) {
//                int pixel = (int) (255 * prob[gray.at<uchar>(i, j)]);
//                gray.at<uchar>(i, j) = pixel;
//            }
//        }
        cv::equalizeHist(gray, gray);
        return gray;
    }


//    1. 计算源图像的累计直方图；
//    2. 计算规定图像的累计直方图；
//    3. 计算源图像累计直方图各个灰度阶到规定图像的累计直方图各个灰度阶的差的绝对值；
//    4. 求出步骤3中各阶中绝对值对应的最小值，最小值对应的灰度阶即为映射后的值。
    /**
     * 直方图匹配
     * @brief histogram_mattch
     * @param src 原图像
     * @param dst 需要匹配的图像
     * @return
     */
    cv::Mat histogram_mattch(const cv::Mat& src, const cv::Mat& style) {
        cv::Mat dst;
        if (src.size() != style.size()) {
            cv::resize(style, dst, src.size());
        } else {
            dst = style;
        }
        cv::Mat out;
        std::vector<cv::Mat> mv_src;
        std::vector<cv::Mat> mv_dst;
        std::vector<cv::Mat> mv_out;
        cv::split(src, mv_src);
        cv::split(dst, mv_dst);
        // 每个通道都做匹配
        for (size_t c = 0; c < src.channels(); c++) {
            // 计算原图和目的图的直方图
            std::vector<float> src_hist = calHist(mv_src[c]);
            std::vector<float> dst_hist = calHist(mv_dst[c]);
            //生成LUT映射表 灰阶映射关系
            cv::Mat lut(1, 256, CV_8U);
            // 根据概率密度最接近的灰阶映射
            for (size_t i = 0; i < src_hist.size(); i++) {
                float min_diff = 1.1;
                for (size_t j = 0; j < dst_hist.size(); j++) {
                    float diff = std::abs(src_hist[i] - dst_hist[j]);
                    if (diff < min_diff) {
                        min_diff = diff;
                        lut.at<uchar>(i) = j;
                    }
                }

            }
            cv::Mat channel(mv_src[c].size(), CV_8UC1);
            // 映射
            cv::LUT(mv_src[c], lut, channel);
            mv_out.push_back(channel);
        }
        // BGR 合成
        cv::merge(mv_out, out);
        return out;
    }

    /**
     * 计算灰阶L的概率密度
     * @brief calHist
     * @param gray
     * @return vector<float>  prob[i]为灰度0-i的概率密度总和
     */
    std::vector<float> calHist(const cv::Mat& gray) {
        std::vector<float> prob(256);
        for (size_t i = 0; i < gray.rows; i++) {
            for (size_t j = 0; j < gray.cols; j++) {
                prob[gray.at<uchar>(i, j)] += 1;
            }
        }
        int MN = gray.rows * gray.cols;
        prob[0] = prob[0] / MN;
        for (size_t i = 1; i < prob.size(); i++) {
            prob[i] = prob[i] / MN;
            prob[i] += prob[i-1];
        }
        return prob;
    }

}




