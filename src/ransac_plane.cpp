#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

struct Plane
{
    double a, b, c, d;
};

// 3点から平面を作る
Plane fitPlane(const cv::Point3d &p1,
               const cv::Point3d &p2,
               const cv::Point3d &p3)
{
    cv::Point3d v1 = p2 - p1;
    cv::Point3d v2 = p3 - p1;

    cv::Point3d n = v1.cross(v2);

    Plane plane;
    plane.a = n.x;
    plane.b = n.y;
    plane.c = n.z;
    plane.d = -(n.x * p1.x + n.y * p1.y + n.z * p1.z);

    return plane;
}

// 点と平面の距離
double pointPlaneDist(const Plane &pl, const cv::Point3d &p)
{
    return std::abs(pl.a * p.x + pl.b * p.y + pl.c * p.z + pl.d) /
           std::sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);
}

int main()
{
    cv::Mat img = cv::imread("../img/load.jpg");
    if (img.empty())
    {
        std::cerr << "image not found" << std::endl;
        return -1;
    }

    // ===== 前処理 =====
    cv::Mat gray, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 80, 160);

    // ===== edge → 擬似3D点群 =====
    std::vector<cv::Point3d> points;

    for (int y = 0; y < edges.rows; y++)
    {
        for (int x = 0; x < edges.cols; x++)
        {
            if (edges.at<uchar>(y, x) > 0)
            {
                points.emplace_back(x, y, 1.0);
            }
        }
    }

    if (points.size() < 10)
    {
        std::cerr << "too few points" << std::endl;
        return -1;
    }

    // ===== RANSAC =====
    std::random_device rd;
    std::mt19937 gen(rd());

    Plane bestPlane;
    int bestInliers = 0;

    for (int iter = 0; iter < 200; iter++)
    {
        std::uniform_int_distribution<> dis(0, points.size() - 1);

        cv::Point3d p1 = points[dis(gen)];
        cv::Point3d p2 = points[dis(gen)];
        cv::Point3d p3 = points[dis(gen)];

        Plane plane = fitPlane(p1, p2, p3);

        int inliers = 0;
        for (const auto &p : points)
        {
            if (pointPlaneDist(plane, p) < 1.0)
            {
                inliers++;
            }
        }

        if (inliers > bestInliers)
        {
            bestInliers = inliers;
            bestPlane = plane;
        }
    }

    std::cout << "Best plane: "
              << bestPlane.a << "x + "
              << bestPlane.b << "y + "
              << bestPlane.c << "z + "
              << bestPlane.d << " = 0\n";

    std::cout << "Inliers: " << bestInliers << std::endl;

    // =========================================================
    // 🔥 可視化部分
    // =========================================================

    cv::Mat vis = img.clone();
    double thresh = 1.0;

    // 点の可視化（inlier / outlier）
    for (const auto &p : points)
    {
        double d = pointPlaneDist(bestPlane, p);

        cv::Point pt((int)p.x, (int)p.y);

        if (d < thresh)
        {
            // inlier → 赤
            cv::circle(vis, pt, 1, cv::Scalar(0, 0, 255), -1);
        }
        else
        {
            // outlier → 青
            cv::circle(vis, pt, 1, cv::Scalar(255, 0, 0), -1);
        }
    }

    // ===== 擬似平面ライン（z=1仮定）=====
    double A = bestPlane.a;
    double B = bestPlane.b;
    double C = bestPlane.c + bestPlane.d;

    if (std::abs(B) > 1e-6)
    {
        cv::Point p1(0, (int)(-C / B));
        cv::Point p2(img.cols, (int)(-(A * img.cols + C) / B));

        cv::line(vis, p1, p2, cv::Scalar(0, 255, 0), 2);
    }

    // ===== 表示 =====
    cv::imshow("edges", edges);
    cv::imshow("RANSAC plane visualization", vis);

    cv::waitKey(0);
    return 0;
}