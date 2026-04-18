#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string win_src = "lenna.png";
string win_dst = "contrast_enhancement.png";
string file_path = "./img/";

int main()
{
    string file_src = file_path + win_src; // 入力画像のファイル名
    string file_dst = file_path + win_dst; // 出力画像のファイル名
    // 入力画像（カラー）の読み込み
    Mat img_src = imread(file_src, IMREAD_COLOR);
    // 入力画像（グレースケール）の読み込み
    // Mat img_src = imread(file_src, IMREAD_GRAYSCALE);

    Mat img_dst;
    if (!img_src.data)
    {
        cout << "error" << endl;
        return -1;
    }

    // ウィンドウ生成
    namedWindow(win_src, WINDOW_AUTOSIZE);
    namedWindow(win_dst, WINDOW_AUTOSIZE);

    // コントラスト低減
    int min = 150, max = 200;
    img_src.convertTo(img_dst, img_src.type(), 255.0 / (max - min), -255.0 * min / (max - min));

    imshow(win_src, img_src);   // 入力画像を表示
    imshow(win_dst, img_dst);   // 出力画像を表示
    imwrite(file_dst, img_dst); // 処理結果の保存

    waitKey(0); // キー入力待ち
    return 0;
}
