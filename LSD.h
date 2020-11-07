#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/ximgproc.hpp>
#include<vector>
const int LINE_THRESHOLD = 50;
using namespace std;

float dist2Point(int x1, int y1, int x2, int y2)
{
	//To compute the length of line segment
	return std::sqrt(double(x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

vector<cv::Vec4f> lsdGetLines(const cv::Mat src)
{
	cv::Mat tmp = src.clone();
	if (src.type() != CV_8UC1)
	{
		cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
	}
	vector<cv::Vec4f> lines, res;

	cv::Ptr<cv::ximgproc::FastLineDetector> detector = cv::ximgproc::createFastLineDetector();
	detector->detect(tmp, lines);
	for (int i = 0; i < lines.size(); ++i)
	{
		//delete line segments whose length is under the threshold
		if (dist2Point(lines[i][0], lines[i][1], lines[i][2], lines[i][3]) > LINE_THRESHOLD) res.push_back(lines[i]);

	}
	return res;
}

