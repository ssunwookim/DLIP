/*

Assigment : [LAB] Grayscale Image Segmentation
Author : 22000090 / SUN-WOO KIM
Day : 2025 / 04 /01

*/


#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat src[4], dst[4], dst_morph, dst_morph_color, Out1[4], Out2[4], Out, Out3[4], Out4[4];

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    for (int i = 0; i < 4; i++) {
        string filename = "Gear" + to_string(i + 1) + ".jpg";
        src[i] = imread(filename, IMREAD_GRAYSCALE);
        Out1[i] = src[i];

        string windowname1 = "Out1-" + to_string(i + 1);
        namedWindow(windowname1, WINDOW_NORMAL);
        imshow(windowname1, Out1[i]);

        string Quality = "Pass";

        if (src[i].empty()) {
            cout << "이미지를 불러오지 못했습니다: " << filename << endl;
            return -1;
        }

        // 이진화
        threshold(src[i], src[i], 127, 255, THRESH_BINARY);

        // 윤곽선 찾음
        findContours(src[i], contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // mask 정의
        Mat mask = Mat::zeros(src[i].size(), CV_8UC1);

        float radius = 0;

        for (size_t i = 0; i < contours.size(); i++) {
            Point2f center;

            minEnclosingCircle(contours[i], center, radius);
            // 기어 이만 남도록 가운데 가릴 원 찾음
            radius = (radius * 4.02) / 5;
            circle(mask, center, radius, Scalar(255, 255, 255), FILLED);
        }

        Mat result = Mat::zeros(src[i].size(), CV_8UC1);
        subtract(src[i], mask, result);

        // 윤곽선 찾기
        contours.clear();
        hierarchy.clear();
        findContours(result, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        //윤곽선 그리기
        Mat drawing = Mat::zeros(src[i].size(), CV_8UC1);
        cvtColor(drawing, drawing, COLOR_GRAY2BGR);
        drawContours(drawing, contours, -1, Scalar(0, 255, 0), 2);
        dst[i] = drawing;

        //윤곽선 면적 계산 및 출력 contourArea
        int Area[20], cx[20], cy[20], Avg_Area = 0;


        for (size_t j = 0; j < contours.size(); j++) {
            Area[j] = contourArea(contours[j]);
            Avg_Area = Avg_Area + Area[j];
            Moments M = moments(contours[j]);
            cx[j] = (int)(M.m10 / M.m00);
            cy[j] = (int)(M.m01 / M.m00);
        }
        Avg_Area = Avg_Area / contours.size();



        // 중앙값 기준으로 오차나는 기어 이빨 
        sort(Area, Area + 19);
        int median = Area[9];
        int min = median * 0.8;
        int max = median * 1.3;

        for (size_t k = 0; k < contours.size(); k++) {
            if (contourArea(contours[k]) > max || contourArea(contours[k]) < min) {
                drawContours(drawing, contours, k, Scalar(0, 0, 255), 2);
            }
        }


        Out3[i] = drawing.clone();


        for (size_t t = 0; t < contours.size(); t++) {
            String Area_st = to_string(Area[t]);
            putText(dst[i], Area_st, Point(cx[t], cy[t]), 1, 1, Scalar(255, 255, 255), 2, 3);
        }

        Out2[i] = drawing.clone();
        string windowname2 = "Out2-" + to_string(i + 1);
        namedWindow(windowname2, WINDOW_NORMAL);
        imshow(windowname2, Out2[i]);

        string windowname3 = "Out3-" + to_string(i + 1);
        namedWindow(windowname3, WINDOW_NORMAL);
        imshow(windowname3, Out3[i]);

        int Badgears = 0;

        for (size_t r = 0; r < contours.size(); r++) {

            if (contourArea(contours[r]) > max || contourArea(contours[r]) < min) {

                for (int angle = 0; angle < 360; angle += 20) {
                    ellipse(Out3[i], Point(cx[r], cy[r]), Size(30, 30), 0, angle, angle + 10, Scalar(0, 255, 255), 2);
                }

                Badgears++;
                Quality = "Fail";
            }
        }

        Out4[i] = Out3[i];
        string windowname4 = "Out4-" + to_string(i + 1);
        namedWindow(windowname4, WINDOW_NORMAL);
        imshow(windowname4, Out4[i]);

        // 기어번호
        cout << "Gear Number : " << "Grea NO." << to_string(i + 1) << endl;
        //검출된 윤곽선 개수 출력
        cout << "Number of contours : " << contours.size() << endl;
        //윤곽선 면적 평균
        cout << "Avg, Teeth Area : " << Avg_Area << endl;
        // 불량 치아 수
        cout << "Bad gears : " << Badgears << endl;
        // 기어 직경
        cout << "Root diameter : " << radius << endl;
        // 품질 검사 
        cout << "Quality : " << Quality << endl;

    }

    /*
    Mat dst_morph_first = dst[0];

    for (int z = 0; z < 20; z++) {
        int n = 40 - (2 * z);
        Mat element = getStructuringElement(MORPH_RECT, Size(n, n));
        morphologyEx(dst_morph_first, dst_morph, MORPH_OPEN, element);
        dst_morph_first = dst_morph;

    }
    */

    waitKey(0);
    return 0;
}
