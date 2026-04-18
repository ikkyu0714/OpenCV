#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

struct Plane
{
    double a, b, c, d;
};

// =========================
// ① 平面フィット（変更なし）
// =========================
Plane fitPlane(const cv::Point3d &p1,
               const cv::Point3d &p2,
               const cv::Point3d &p3)
{
    cv::Point3d v1 = p2 - p1;
    cv::Point3d v2 = p3 - p1;
    cv::Point3d n = v1.cross(v2);

    Plane pl;
    pl.a = n.x;
    pl.b = n.y;
    pl.c = n.z;
    pl.d = -(n.x * p1.x + n.y * p1.y + n.z * p1.z);
    return pl;
}

// =========================
// ② 距離関数（変更なし）
// =========================
double dist(const Plane &pl, const cv::Point3d &p)
{
    return std::abs(pl.a * p.x + pl.b * p.y + pl.c * p.z + pl.d) /
           std::sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
}

int main()
{
    cv::Mat img = cv::imread("../img/room.jpg");
    if (img.empty())
        return -1;

    cv::Mat gray, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 80, 160);

    // =========================================================
    // ③ edge → 擬似3D点群（変更なし）
    // =========================================================
    std::vector<cv::Point3d> pts;
    for (int y = 0; y < edges.rows; y++)
    {
        for (int x = 0; x < edges.cols; x++)
        {
            if (edges.at<uchar>(y, x) > 0)
            {
                pts.emplace_back(x, y, 1.0);
            }
        }
    }

    // =========================================================
    // ④ RANSAC plane（変更なし）
    // =========================================================
    std::mt19937 gen(0);
    std::uniform_int_distribution<> dis(0, pts.size() - 1);

    Plane best;
    int bestInliers = 0;

    for (int i = 0; i < 150; i++)
    {
        Plane pl = fitPlane(
            pts[dis(gen)],
            pts[dis(gen)],
            pts[dis(gen)]);

        int inliers = 0;
        for (auto &p : pts)
        {
            if (dist(pl, p) < 1.0)
                inliers++;
        }

        if (inliers > bestInliers)
        {
            bestInliers = inliers;
            best = pl;
        }
    }

    std::cout << "Plane found: "
              << best.a << "x + "
              << best.b << "y + "
              << best.c << "z + "
              << best.d << std::endl;

    // =========================================================
    // ❌❌❌【削除された旧処理】❌❌❌
    // boundingRectでHomography（RANSAC未使用）
    //
    // std::vector<cv::Point> edgePts;
    // cv::Rect bbox = cv::boundingRect(edgePts);
    // src = bbox四隅
    // =========================================================

    // =========================================================
    // ✔️ 新処理①：RANSAC inlier抽出
    // =========================================================
    std::vector<cv::Point2f> inliers2D;
    double thresh = 1.0;

    for (const auto &p : pts)
    {
        if (dist(best, p) < thresh)
        {
            inliers2D.emplace_back(p.x, p.y);
        }
    }

    // =========================================================
    // ✔️ 新処理②：凸包（形状安定化）
    // =========================================================
    std::vector<cv::Point2f> hull;
    cv::convexHull(inliers2D, hull);

    // =========================================================
    // ✔️ 新処理③：4点化（Homography用）
    // =========================================================
    std::vector<cv::Point2f> src;

    cv::approxPolyDP(hull, src, 0.02 * cv::arcLength(hull, true), true);

    // 念のため4点制約（簡易フォールバック）
    if (src.size() != 4)
    {
        cv::Rect bbox = cv::boundingRect(hull);
        src = {
            {(float)bbox.x, (float)bbox.y},
            {(float)(bbox.x + bbox.width), (float)bbox.y},
            {(float)(bbox.x + bbox.width), (float)(bbox.y + bbox.height)},
            {(float)bbox.x, (float)(bbox.y + bbox.height)}};
    }

    // =========================================================
    // ✔️ 新処理④：目的座標（鳥瞰）
    // =========================================================
    int W = 500, H = 700;
    std::vector<cv::Point2f> dst = {
        {0, 0}, {float(W), 0}, {float(W), float(H)}, {0, float(H)}};

    // =========================================================
    // ✔️ Homography（ここは変更なし）
    // =========================================================
    cv::Mat Hmat = cv::findHomography(src, dst);
    cv::Mat warped;
    cv::warpPerspective(img, warped, Hmat, cv::Size(W, H));

    // =========================================================
    // ✔️ 可視化（追加）
    // =========================================================
    cv::Mat vis = img.clone();

    for (const auto &p : pts)
    {
        double d = dist(best, p);
        cv::circle(vis, cv::Point(p.x, p.y), 1,
                   d < thresh ? cv::Scalar(0, 0, 255)
                              : cv::Scalar(255, 0, 0),
                   -1);
    }

    cv::imshow("edges", edges);
    cv::imshow("RANSAC inliers + hull", vis);
    cv::imshow("bird view", warped);

    cv::waitKey(0);
}