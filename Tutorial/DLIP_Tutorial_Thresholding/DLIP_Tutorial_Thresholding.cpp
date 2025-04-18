#include "iostream"
#include <opencv.hpp>

using namespace std;
using namespace cv;

// Function headers
void plotHist(Mat src, string plotname, int width, int height);

// Global variables for Morphology
int element_shape = MORPH_RECT;		// MORPH_RECT, MORPH_ELIPSE, MORPH_CROSS
int n = 3;
Mat element = getStructuringElement(element_shape, Size(n, n));

int main() {
	
	Mat src, src_gray, dst, dst_morph;

	src = imread("../../Image/testimage/localThresh3.jpg", 0);    // Load an image (DLIP/Image/coin.jpg)

	if (src.empty())					// Load image check
	{
		cout << "File Read Failed : src is empty" << endl;
		waitKey(0);
	}
	// Create a window to display results
	namedWindow("DemoWIndow", WINDOW_AUTOSIZE); //WINDOW_AUTOSIZE(1) :Fixed Window, 0: Unfixed window
	if (!src.empty())imshow("DemoWIndow", src); // Show image


	/* threshold_type
	0: Binary
	1: Binary Inverted
	2: Threshold Truncated
	3: Threshold to Zero
	4: Threshold to Zero Inverted*/
	int threshold_value = 180;
	int threshold_type = 8;
	int const max_value = 255;
	int const max_type = 4;
	int const max_BINARY_value = 255;

	threshold(src, dst, threshold_value, max_BINARY_value, threshold_type);

	Mat adaptiveThreshold_dst;
	int const blocksize = 3;
	adaptiveThreshold(src, adaptiveThreshold_dst, max_value, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, blocksize, 0);

	morphologyEx(adaptiveThreshold_dst, dst_morph, MORPH_CLOSE, element);

	// Plot histogram
	string plotname = "Histogram";
	plotHist(dst, plotname, 500, 500);

	// Create a window to display results
	namedWindow("ThreshWIndow", WINDOW_AUTOSIZE); //WINDOW_AUTOSIZE(1) :Fixed Window, 0: Unfixed window
	imshow("ThreshWIndow", dst); // Show image
	imshow("adaptiveThreshold", adaptiveThreshold_dst);
	imshow("mophology", dst_morph);

	waitKey(0);//Pause the program
	return 0;

}

void plotHist(Mat src, string plotname, int width, int height) {
	/// Compute the histograms 
	Mat hist;
	/// Establish the number of bins (for uchar Mat type)
	int histSize = 256;
	/// Set the ranges (for uchar Mat type)
	float range[] = { 0, 256 };

	const float* histRange = { range };
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	double min_val, max_val;
	cv::minMaxLoc(hist, &min_val, &max_val);
	Mat hist_normed = hist * height / max_val;
	float bin_w = (float)width / histSize;
	Mat histImage(height, width, CV_8UC1, Scalar(0));
	for (int i = 0; i < histSize - 1; i++) {
		line(histImage,
			Point((int)(bin_w * i), height - cvRound(hist_normed.at<float>(i, 0))),
			Point((int)(bin_w * (i + 1)), height - cvRound(hist_normed.at<float>(i + 1, 0))),
			Scalar(255), 2, 8, 0);
	}

	imshow(plotname, histImage);
}