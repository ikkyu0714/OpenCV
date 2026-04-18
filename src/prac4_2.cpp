#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    const int width = 9, height = 9;
    unsigned char img[height * width];     // 画像配列用
    string filename = "../img/output.pgm"; // ファイル名
    ofstream fout(filename);               // ファイルを開く

    // 画像生成
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int pixel_value = (x + y) * 32;
            if (pixel_value == 256)
            {
                pixel_value = 255;
            }
            else if (pixel_value > 255)
            {
                pixel_value = 257 - (pixel_value - 255);
            }

            img[y * width + x] = pixel_value;
        }
    }

    // ファイル出力
    fout << "P2" << endl;
    fout << width << " " << height << endl;
    fout << "255" << endl;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            fout << (int)img[y * width + x] << " ";
        }
        fout << endl;
    }
    fout.flush(); // ファイル保存
    fout.close(); // ファイルクローズ

    return 0;
}