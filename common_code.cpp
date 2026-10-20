#include "common_code.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cv::Mat fsiv_color_rescaling(const cv::Mat &in, const cv::Scalar &from, const cv::Scalar &to)
{
    cv::Mat out;
    cv::Scalar factor;
    cv::divide(to, from, factor);
    out = in.mul(factor);
    CV_Assert(out.type() == in.type());
    CV_Assert(out.size() == in.size());
    return out;
}

cv::Mat fsiv_gray_world_color_balance(cv::Mat const &in)
{
    CV_Assert(in.type() == CV_8UC3);
    cv::Mat out;
    auto v_mean = cv::mean(in);
    out = fsiv_color_rescaling(in, v_mean, cv::Scalar{128.0, 128.0, 128.0});
    CV_Assert(out.type() == in.type());
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    return out;
}

cv::Mat fsiv_convert_bgr_to_gray(const cv::Mat &img, cv::Mat &out)
{
    CV_Assert(img.channels() == 3);
    cv::cvtColor(img, out, cv::COLOR_BGR2GRAY);
    CV_Assert(out.channels() == 1);
    return out;
}

cv::Mat fsiv_compute_image_histogram(cv::Mat const &img)
{
    CV_Assert(img.type() == CV_8UC1);
    cv::Mat hist;
    std::vector<cv::Mat> images {img};
    std::vector<int> channels {0};
    std::vector<int> histSize {256};
    std::vector<float> ranges {0.0, 256.0};
    cv::calcHist(images, channels, cv::Mat(), hist, histSize, ranges, false);
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.rows == 256 && hist.cols == 1);
    return hist;
}

int fsiv_compute_histogram_percentile(cv::Mat const &hist, float p_value)
{
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.cols == 1);
    CV_Assert(0.0 <= p_value && p_value <= 1.0);

    int p = 0;

    float target_area = p_value * cv::sum(hist)[0];
    float area = 0;

    for(p; area < target_area && p < hist.rows; p++)
    {
        area += hist.at<float>(p);
    }

    if (p > 0)
        p--;

    CV_Assert(0 <= p && p < hist.rows);
    return p;
}

cv::Mat fsiv_white_patch_color_balance(cv::Mat const &in, float p)
{
    CV_Assert(in.type() == CV_8UC3);
    CV_Assert(0.0f <= p && p < 1.0f);
    cv::Mat out;
    if (p == 0.0)
    {
        cv::Mat luma;
        luma = fsiv_convert_bgr_to_gray(in, luma);
        cv::Point max_loc;
        cv::minMaxLoc(luma, nullptr, nullptr, nullptr, &max_loc);
        cv::Scalar max_rgb = in.at<cv::Vec3b>(max_loc.y, max_loc.x);
        out = fsiv_color_rescaling(in, max_rgb, cv::Scalar::all(255.0));
    }
    else
    {
        cv::Mat luma;
        luma = fsiv_convert_bgr_to_gray(in, luma);
        cv::Mat hist = fsiv_compute_image_histogram(luma);
        int p_v = fsiv_compute_histogram_percentile(hist, 1.0f - p);
        cv::Mat mask = luma >= p_v;
        cv::Scalar from = cv::mean(in, mask);
        out = fsiv_color_rescaling(in, from, cv::Scalar::all(255.0));
    }

    CV_Assert(out.type() == in.type());
    CV_Assert(out.rows == in.rows && out.cols == in.cols);
    return out;
}
