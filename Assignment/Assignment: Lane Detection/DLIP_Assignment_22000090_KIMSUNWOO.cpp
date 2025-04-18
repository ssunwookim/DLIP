/*
Author : 22000090 KIM SUNWOO
Date   : 2024 / 04 / 07
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// functions
Mat applyROI(Mat img);        // ROI 관심 영역 외 마스킹
void LaneDetection(Mat& src, const string& windowName); // 차선 인식 함수

int main() {
    Mat Out1, Out2;

    // 이미지 불러옴
    Out1 = imread("Lane_center.jpg");
    Out2 = imread("Lane_changing.jpg");

    LaneDetection(Out1, "Image 1");
    LaneDetection(Out2, "Image 2");

    // 결과 출력
    imshow("OUTPUT 1", Out1);
    imshow("OUTPUT 2", Out2);

    waitKey(0);
    return 0;
}

Mat applyROI(Mat img) {
    Mat mask = Mat::zeros(img.size(), img.type());

    // 관심 영역 설정 (사다리꼴)
    Point pts[1][4];
    int h = img.rows;
    int w = img.cols;

    pts[0][0] = Point(w * 0.1, h);
    pts[0][1] = Point(w * 0.45, h * 0.6);
    pts[0][2] = Point(w * 0.55, h * 0.6);
    pts[0][3] = Point(w * 0.9, h);

    const Point* ppt[1] = { pts[0] };
    int npt[] = { 4 };
    fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255));

    Mat roi;
    bitwise_and(img, mask, roi);

    return roi;
}

void LaneDetection(Mat& src, const string& windowName) {
    // 가우시안 블러
    GaussianBlur(src, src, Size(5, 5), 0);

    // Canny 엣지
    Mat edge;
    Canny(src, edge, 150, 200, 3);
    imshow(windowName + " - Canny", edge); // Canny 결과 출력

    // ROI 마스킹
    Mat roi = applyROI(edge);
    edge = roi;

    // Hough Transform
    vector<Vec4i> lines;
    HoughLinesP(edge, lines, 1, CV_PI / 180, 20, 25, 10);

    // Hough 결과 이미지 복사본 생성 및 그리기
    Mat hough_result;
    cvtColor(edge, hough_result, COLOR_GRAY2BGR);
    for (const auto& line : lines) {
        cv::line(hough_result, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(0, 255, 255), 2);
    }
    imshow(windowName + " - HoughLinesP", hough_result); // Hough 결과 출력

    // 좌우 차선 분류
    vector<Point> left_pts, right_pts;
    for (const auto& line : lines) {
        Point p1(line[0], line[1]);
        Point p2(line[2], line[3]);

        double dx = p2.x - p1.x;
        if (dx == 0) continue;

        double slope = (double)(p2.y - p1.y) / dx;
        if (abs(slope) < 0.3) continue;

        if (slope < 0) {
            left_pts.push_back(p1);
            left_pts.push_back(p2);
        }
        else {
            right_pts.push_back(p1);
            right_pts.push_back(p2);
        }
    }

    // 직선 피팅
    Vec4f left_line, right_line;
    fitLine(left_pts, left_line, DIST_L2, 0, 0.01, 0.01);
    fitLine(right_pts, right_line, DIST_L2, 0, 0.01, 0.01);

    // 소실점 계산
    double l_slope = left_line[1] / left_line[0];
    double l_intercept = left_line[3] - l_slope * left_line[2];
    double r_slope = right_line[1] / right_line[0];
    double r_intercept = right_line[3] - r_slope * right_line[2];

    int px = (int)((r_intercept - l_intercept) / (l_slope - r_slope));
    int py = (int)(l_slope * px + l_intercept);
    circle(src, Point(px, py), 8, Scalar(255, 200, 255), 2);

    int height = src.rows;

    // 왼쪽 차선
    int y1_l = py;
    int y2_l = height;
    int x1_l = (y1_l - l_intercept) / l_slope;
    int x2_l = (y2_l - l_intercept) / l_slope;
    line(src, Point(x1_l, y1_l), Point(x2_l, y2_l), Scalar(0, 0, 255), 3);

    // 오른쪽 차선
    int y1_r = py;
    int y2_r = height;
    int x1_r = (y1_r - r_intercept) / r_slope;
    int x2_r = (y2_r - r_intercept) / r_slope;
    line(src, Point(x1_r, y1_r), Point(x2_r, y2_r), Scalar(0, 255, 0), 3);

    // 소실점에서 수직선
    line(src, Point(px, src.rows), Point(px, py), Scalar(255, 0, 0), 2);
}
