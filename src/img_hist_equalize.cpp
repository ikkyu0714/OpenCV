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

    // ヒストグラム表示用, 256*100ピクセル, 0（黒）で初期化
    Mat img_hst, img_dst;
    img_hst = Mat::zeros(100, 256, CV_8UC1);

    const int hdims[] = {256}; // 次元ごとの度数分布のサイズ
    const float ranges[] = {0, 256};
    const float *hranges[] = {ranges}; // 次元ごとのビンの下限上限

    Mat gray;
    cvtColor(img_src, gray, COLOR_BGR2GRAY);

    equalizeHist(gray, img_dst);

    // 1チャンネル画像の度数分布を計算
    Mat hist;
    calcHist(&img_dst, 1, 0, Mat(), hist, 1, hdims, hranges);
    // calcHist(&img_src, 1, 0, Mat(), hist, 1, hdims, hranges);

    // 度数の最大値を取得
    double hist_min, hist_max;
    minMaxLoc(hist, &hist_min, &hist_max);

    // ヒストグラムを白線で描画
    for (int i = 0; i <= 255; i++)
    {
        int v = saturate_cast<int>(hist.at<float>(i));
        cout << i << " " << v << endl;
        line(img_hst, Point(i, img_hst.rows), Point(i, img_hst.rows - img_hst.rows * (v / hist_max)), Scalar(255, 255, 255));
    }

    // imshow("Histogram", img_hst);
    imshow("Histogram", img_hst);
    waitKey(0);
    return 0;
}
