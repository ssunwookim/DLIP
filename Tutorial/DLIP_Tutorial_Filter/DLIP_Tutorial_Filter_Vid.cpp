/*------------------------------------------------------/
* Image Proccessing with Deep Learning
* OpenCV : Filter Demo - Video
* Created: 2021-Spring
------------------------------------------------------*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	/*  open the video camera no.0  */
	VideoCapture cap(0);

	if (!cap.isOpened())	// if not success, exit the programm
	{
		cout << "Cannot open the video cam\n";
		return -1;
	}

	namedWindow("MyVideo", WINDOW_AUTOSIZE);

	int key = 0;
	int kernel_size = 11;
	int filter_type = 4;
	j
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	while (1)
	{
		Mat src, dst, result_laplcaian;

		/*  read a new frame from video  */
		bool bSuccess = cap.read(src);

		if (!bSuccess)	// if not success, break loop
		{
			cout << "Cannot find a frame from  video stream\n";
			break;
		}


		key = waitKey(30);
		if (key == 27) // wait for 'ESC' press for 30ms. If 'ESC' is pressed, break loop
		{
			cout << "ESC key is pressed by user\n";
			break;
		}
		else if (key == 'b' || key == 'B')
		{
			filter_type = 1;	// blur
		}
		else if (key == 'g' || key == 'G')
		{
			filter_type = 2; // Gaussian Blur
		}
		else if (key == 'm' || key == 'M')
		{
			filter_type = 3; // Median Filter
		}
		else if (key == 'l' || key == 'L')
		{
			filter_type = 4; // Laplacian Filter
		}


		if (filter_type == 1)
			blur(src, dst, cv::Size(kernel_size, kernel_size), cv::Point(-1, -1));
		else if (filter_type == 2)
			GaussianBlur(src, dst, Size(kernel_size, kernel_size), 0, 0);
		else if (filter_type == 3)
			medianBlur(src, dst, kernel_size);
		else if (filter_type == 4) 
		{
			cv::Laplacian(src, dst, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT);
			src.convertTo(src, CV_16S);
			result_laplcaian = src + dst;
			result_laplcaian.convertTo(result_laplcaian, CV_8U);
			dst = result_laplcaian;
		}
		else
			src.copyTo(dst);

			imshow("MyVideo", result_laplcaian);
	}

	return 0;
}