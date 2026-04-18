#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    string file_src = "./img/lenna.png"; // 入力画像のファイル名
    // 入力画像（カラー）の読み込み
    Mat img_src = imread(file_src, IMREAD_COLOR);
    // 入力画像（グレースケール）の読み込み
    Mat img_gray = imread(file_src, IMREAD_GRAYSCALE);

    // ウィンドウ生成
    namedWindow("src", WINDOW_AUTOSIZE);
    namedWindow("gray", WINDOW_AUTOSIZE);

    int x = 10, y = 50, v = 25, r = 255, g = 255, b = 0;

    // グレースケール画像の場合
    cout << (int)img_gray.at<uchar>(y, x) << endl;
    img_gray.at<uchar>(y, x) = 255;
    cout << (int)img_gray.at<uchar>(y, x) << endl;

    // カラー画像の場合
    cout << img_src.at<Vec3b>(y, x) << endl;
    img_src.at<Vec3b>(y, x) = Vec3b(b, g, r);
    cout << img_src.at<Vec3b>(y, x) << endl;

    imshow("src", img_src);   // 入力画像を表示
    imshow("gray", img_gray); // 出力画像を表示

    waitKey(0); // キー入力待ち
    return 0;
}
