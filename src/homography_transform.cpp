#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::Mat img = cv::imread("../img/room.jpg");
    if (img.empty())
        return -1;

    int W = 500;
    int H = 700;

    // ===== ① 画像上の4点（例：台形）=====
    std::vector<cv::Point2f> srcPts = {
        cv::Point2f(214, 263), // 左上
        cv::Point2f(351, 258), // 右上
        cv::Point2f(421, 355), // 右下
        cv::Point2f(184, 353)  // 左下
    };

    // ===== ② 鳥瞰（正面）の4点 =====
    std::vector<cv::Point2f> dstPts = {
        cv::Point2f(0, 0),
        cv::Point2f(W, 0),
        cv::Point2f(W, H),
        cv::Point2f(0, H)};

    // ===== ③ Homography =====
    cv::Mat Hmat = cv::findHomography(srcPts, dstPts);

    // ===== ④ warp =====
    cv::Mat birdview;
    cv::warpPerspective(img, birdview, Hmat, cv::Size(W, H));

    cv::imshow("input", img);
    cv::imshow("bird's eye view", birdview);

    cv::waitKey(0);
}