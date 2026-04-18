#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::Mat img = cv::Mat::zeros(400, 400, CV_8UC3);

    cv::circle(img, {200, 200}, 100, {0, 255, 0}, 5);

    cv::imshow("test", img);
    cv::waitKey(0);

    return 0;
}