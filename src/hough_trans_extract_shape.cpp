#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::Mat img = cv::imread("../img/gray_calc.jpg");
    if (img.empty())
        return -1;

    cv::Mat gray, blur, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 1.5);
    cv::Canny(blur, edges, 80, 160);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto &c : contours)
    {

        double area = cv::contourArea(c);
        if (area < 500)
            continue; // ノイズ除去

        // 輪郭近似（多角形化）
        std::vector<cv::Point> approx;
        cv::approxPolyDP(c, approx, 0.02 * cv::arcLength(c, true), true);

        // ===== 三角形・四角形など =====
        if (approx.size() == 3)
        {
            cv::polylines(img, approx, true, cv::Scalar(0, 255, 0), 2);
            cv::putText(img, "Triangle", approx[0],
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }

        // ===== 円・楕円判定 =====
        else if (approx.size() > 5)
        {

            // 楕円フィット
            if (c.size() >= 5)
            {
                cv::RotatedRect ellipse = cv::fitEllipse(c);
                cv::ellipse(img, ellipse, cv::Scalar(255, 0, 0), 2);
                cv::putText(img, "Ellipse", ellipse.center,
                            cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 0), 2);
            }
        }

        // ===== 円（HoughCirclesも併用可）=====
        else
        {
            cv::drawContours(img, std::vector<std::vector<cv::Point>>{c}, -1,
                             cv::Scalar(0, 0, 255), 2);
        }
    }

    cv::imshow("result", img);
    cv::imshow("edges", edges);
    cv::waitKey(0);
}