#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

// 全局变量
Mat src;
Mat dst, detected_edges, cdst;
//Canny参数
int lowThreshold_Canny = 20;
int const max_lowThreshold_Canny = 150;
int ratio = 3;
int kernel_size = 3;
int edgeThresh = 1;
//Hough参数
int lowThreshold_Hough = 100;
int const max_lowThreshold_Hough = 200;

char* window_name = "Edge Map";
bool flag = false;// true:HoughLines  false:HoughLinesP

void Operations(int, void*)
{
	//Canny处理&灰度处理
	Canny(src, dst, lowThreshold_Canny, lowThreshold_Canny*ratio, kernel_size);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	
	if (flag)
	{
		vector<Vec2f> lines;
		if (lowThreshold_Hough == 0)
			HoughLines(dst, lines, 1, CV_PI / 180, 1, 0, 0);
		else
			HoughLines(dst, lines, 1, CV_PI / 180, lowThreshold_Hough, 0, 0);

		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(cdst, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
		}
	}
	else
	{
		vector<Vec4i> lines;
		if (lowThreshold_Hough == 0)
			HoughLinesP(dst, lines, 1, CV_PI / 180, 1, 1, 10);
		else
			HoughLinesP(dst, lines, 1, CV_PI / 180, lowThreshold_Hough, lowThreshold_Hough, 10);

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
		}
	}
	imshow(window_name, cdst);
}

int main()
{
	// 装载图像
	src = imread("house.jpg");
	if (!src.data)
	{
		return -1;
	}

	// 创建显示窗口
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	// 创建trackbar
	createTrackbar("Canny Threshold", window_name, &lowThreshold_Canny, max_lowThreshold_Canny, Operations);
	createTrackbar("Hough Threshold", window_name, &lowThreshold_Hough, max_lowThreshold_Hough, Operations);

	// 显示图像
	Operations(0, 0);

	// 等待用户反应
	waitKey(0);

	return 0;
}