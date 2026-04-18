#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // 画像読み込み（グレースケール）
    cv::Mat img = cv::imread("../img/gray_calc.jpg");
    if (img.empty())
    {
        std::cerr << "image not found" << std::endl;
        return -1;
    }

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // ノイズ除去
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);

    // エッジ検出（重要）
    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150);

    // Hough変換（確率的ハフ変換）
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(
        edges,
        lines,
        1,           // rho精度（ピクセル）
        CV_PI / 180, // theta精度（ラジアン）
        50,          // 閾値（投票数）
        50,          // 最小線分長
        10           // ギャップ許容
    );

    // 線を描画
    for (const auto &l : lines)
    {
        cv::line(img,
                 cv::Point(l[0], l[1]),
                 cv::Point(l[2], l[3]),
                 cv::Scalar(0, 0, 255),
                 2);
    }

    // 表示
    cv::imshow("edges", edges);
    cv::imshow("lines", img);

    cv::waitKey(0);
    return 0;
}