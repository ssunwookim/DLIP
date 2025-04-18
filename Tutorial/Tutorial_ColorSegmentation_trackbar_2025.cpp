//#include "opencv2/video/tracking.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <ctype.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat image;
Point origin;
Rect selection;
bool selectObject = false;
bool trackObject = false;

//int hmin = 16, hmax = 17, smin = 30, smax = 255, vmin = 115, vmax = 255;


int hmin = 63, hmax = 73, smin = 30, smax = 255, vmin = 115, vmax = 255;

/// On mouse event 
static void onMouse(int event, int x, int y, int, void*);

int main()
{
	Mat image_disp, hsv, hue, mask, dst, result;
	vector<vector<Point> > contours;

	/*  open the video camera no.0  */
	//VideoCapture cap(0);
	//
	//if (!cap.isOpened())	// if not success, exit the programm
	//{
	//	cout << "Cannot open the video cam\n";
	//	return -1;
	//}




	// TrackBar 설정
	namedWindow("Source", 0);
	setMouseCallback("Source", onMouse, 0);
	createTrackbar("Hmin", "Source", &hmin, 179, 0);
	createTrackbar("Hmax", "Source", &hmax, 179, 0);
	createTrackbar("Smin", "Source", &smin, 255, 0);
	createTrackbar("Smax", "Source", &smax, 255, 0);
	createTrackbar("Vmin", "Source", &vmin, 255, 0);
	createTrackbar("Vmax", "Source", &vmax, 255, 0);


	VideoCapture cap("../../Image/222.mp4");

	Mat background;
	cap >> background;

	while (1)
	{
		background.copyTo(mask);

		cap >> image;

		image.copyTo(image_disp);

		Mat dst_track = Mat::zeros(image.size(), CV_8UC3);

		imshow("Source", image);
		/******** Convert BGR to HSV ********/
		// input mat: image
		// output mat: hsv
		cvtColor(image, hsv, COLOR_BGR2HSV); // 색상 변환

		/******** Add Pre-Processing such as filtering etc  ********/
		// YOUR CODE GOES HERE
		// YOUR CODE GOES HERE


		/// set dst as the output of InRange
		inRange(hsv, Scalar(MIN(hmin, hmax), MIN(smin, smax), MIN(vmin, vmax)),
			Scalar(MAX(hmin, hmax), MAX(smin, smax), MAX(vmin, vmax)), dst);


		/******** Add Post-Processing such as morphology etc  ********/
	    // 이진화 (threshold를 사용해서)
		Mat binary;
		threshold(dst, binary, 100, 255, THRESH_BINARY);

		// 팽창 연산을 위한 커널(구조 요소) 정의
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));

		// 팽창 결과를 저장할 Mat
		Mat dilated;
		dilate(binary, dst, kernel);

		namedWindow("InRange", 0);
		imshow("InRange", dst);

		/// once mouse has selected an area bigger than 0
		if (trackObject)
		{
			trackObject = false;					// Terminate the next Analysis loop
			Mat roi_HSV(hsv, selection); 			// Set ROI by the selection box		
			Scalar means, stddev;
			meanStdDev(roi_HSV, means, stddev);
			cout << "\n Selected ROI Means= " << means << " \n stddev= " << stddev;

			// Change the value in the trackbar according to Mean and STD //
			hmin = MAX((means[0] - stddev[0]), 0);
			hmax = MIN((means[0] + stddev[0]), 179);
			setTrackbarPos("Hmin", "Source", hmin);
			setTrackbarPos("Hmax", "Source", hmax);

			/******** Repeat for S and V trackbar ********/
			// S 채널 범위 설정
			smin = MAX((int)(means[1] - stddev[1]), 0);
			smax = MIN((int)(means[1] + stddev[1]), 255);
			setTrackbarPos("Smin", "Source", smin);
			setTrackbarPos("Smax", "Source", smax);

			// V 채널 범위 설정
			vmin = MAX((int)(means[2] - stddev[2]), 0);
			vmax = MIN((int)(means[2] + stddev[2]), 255);
			setTrackbarPos("Vmin", "Source", vmin);
			setTrackbarPos("Vmax", "Source", vmax);

		}


		if (selectObject && selection.area() > 0)  // Left Mouse is being clicked and dragged
		{
			// Mouse Drag을 화면에 보여주기 위함
			Mat roi_RGB(image_disp, selection);
			bitwise_not(roi_RGB, roi_RGB);
			imshow("Source", image_disp);
		}
		image.copyTo(image_disp);

		///  Find All Contour   ///
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

			///  Draw the max Contour on Black-background  Image ///
			Mat dst_out = Mat::zeros(dst.size(), CV_8UC3);
			drawContours(dst_out, contours, maxArea_idx, Scalar(0, 0, 255), 2, 8);
			namedWindow("Contour", 0);
			imshow("Contour", dst_out);

			/// Draw the Contour Box on Original Image ///
			Rect boxPoint = boundingRect(contours[maxArea_idx]);
			rectangle(image_disp, boxPoint, Scalar(0, 0, 0), -1);
			namedWindow("Contour_Box", 0);
			imshow("Contour_Box", image_disp);

			mask = Mat::zeros(background.size(), CV_8UC1);
			rectangle(mask, boxPoint, Scalar(255, 255, 255), -1);
			namedWindow("Contour_Background", 0);
			Mat result = Mat::zeros(mask.size(), CV_8UC3);
			//bitwise_not(mask, mask);
			bitwise_and(background, background, result, mask);

			/// Continue Drawing the Contour Box  ///
			rectangle(dst_track, boxPoint, Scalar(0, 0, 0), -1);
			namedWindow("Contour_Track", 0);
			imshow("Contour_Track", result);

			Mat result1 = Mat::zeros(background.size(), CV_8UC3);
			result1 = result + image_disp;

			imshow("result", result1);

		}

		char c = (char)waitKey(100);
		if (c == 27)
			break;
	
		
	} // end of for(;;)


	return 0;
}



/// On mouse event 
static void onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)  // for any mouse motion
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = abs(x - origin.x) + 1;
		selection.height = abs(y - origin.y) + 1;
		selection &= Rect(0, 0, image.cols, image.rows);  /// Bitwise AND  check selectin is within the image coordinate
	}

	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		selectObject = true;
		origin = Point(x, y);
		break;
	case EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.area())
			trackObject = true;
		break;
	}
}