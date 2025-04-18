/*

Author: 22000090 SUN-WOO KIM
Date: 2025 / 04 / 18

*/

#include "iostream"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int hmin = 16, hmax = 17, smin = 30, smax = 255, vmin = 115, vmax = 255;


//int hmin = 63, hmax = 73, smin = 30, smax = 255, vmin = 115, vmax = 255;


int main() {

	VideoCapture cap("../../Image/LAB_MagicCloak_Sample2.mp4");

	if (!cap.isOpened()) {
		cerr << "비디오를 열 수 없습니다." << endl;
		return -1;
	}

	Mat background, Frame, hsv, dst, binary, mask;

	cap >> background;

	// 저장할 비디오 설정
	Size frameSize = Size((int)cap.get(CAP_PROP_FRAME_WIDTH), (int)cap.get(CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer;
	writer.open("output1.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 50, frameSize, true);

	if (!writer.isOpened()) {
		cerr << "비디오 저장을 위한 VideoWriter 초기화 실패" << endl;
		return -1;
	}

	while (true) {

		Mat Frame, hsv;
		vector<vector<Point> > contours;

		cap >> Frame;
		
		if (Frame.empty()) { // 더이상 읽을 프레임 없으면 종료
			break;
		}
		
		//imshow("Video Frame", Frame);

		//Convert BGR to HSV
		cvtColor(Frame, hsv, COLOR_BGR2HSV);

		//가우시안 블러를 통한 노이즈 제거
		GaussianBlur(hsv, hsv, Size(5, 5), 0);

		//inRange로 바이너리 이미지 만듬
		inRange(hsv, Scalar(MIN(hmin, hmax), MIN(smin, smax), MIN(vmin, vmax)),
			Scalar(MAX(hmin, hmax), MAX(smin, smax), MAX(vmin, vmax)), dst);

		//이진화
		threshold(dst, binary, 100, 255, THRESH_BINARY);

		//dilate
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat dilated;
		dilate(binary, dst, kernel);

		//외곽선 생성
		findContours(dst, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		if (contours.size() > 0)
		{
			/// Find the Contour with the largest area ///
			double maxArea = 0;
			int maxArea_idx = 0;

			for (int i = 0; i < contours.size(); i++)
				if (contourArea(contours[i]) > maxArea) {
					maxArea = contourArea(contours[i]);
					maxArea_idx = i;
				}

			// 원본 이미지에 마스킹 처리
			Rect boxPoint = boundingRect(contours[maxArea_idx]);
			rectangle(Frame, boxPoint, Scalar(0, 0, 0), -1);

			// 검은색 바탕에 원본 이미지에 마스킹 처리 한 부분과 같은 부분에 흰색 마스킹
			mask = Mat::zeros(background.size(), CV_8UC1);
			rectangle(mask, boxPoint, Scalar(255, 255, 255), -1);

			// 배경 이미지와 bitwise and 연산자를 이용하여 특정 부분에 대한 배경 이미지 나타냄
			Mat result = Mat::zeros(mask.size(), CV_8UC3);
			bitwise_and(background, background, result, mask);

			// 원본 이미지 마스킹된 부분에 배경 이미지 합성
			result = result + Frame;

			imshow("result", result);

			writer.write(result);

		}

		if (waitKey(10) == 'q') {
			break;
		}
	
	}

	cap.release(); // 영상 파일 닫음
	writer.release(); // 저장된 영상 파일 닫기

	return 0;
}
