#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat img;

// マウスイベント
void onMouse(int event, int x, int y, int, void *)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        std::cout << "Clicked: (" << x << ", " << y << ")" << std::endl;

        // 点を描画
        cv::circle(img, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);
        cv::imshow("image", img);
    }
}

int main()
{
    img = cv::imread("../img/load.jpg");
    if (img.empty())
        return -1;

    cv::namedWindow("image");
    cv::setMouseCallback("image", onMouse);

    cv::imshow("image", img);
    cv::waitKey(0);
}