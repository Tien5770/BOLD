#pragma once
#include<opencv2/opencv.hpp>
#include<cstring>
#include<string>
#include"BOLDFeature.h"
#include"BOLDVector.h"
const int K_NEAREST_NUM = 5;

struct Line {
	float x1, y1;
	float x2, y2;
	Line(float _x1, float _y1, float _x2, float _y2)
	{
		x1 = _x1;
		x2 = _x2;
		y1 = _y1;
		y2 = _y2;
	}
	Line(cv::Point a, cv::Point b)
	{
		x1 = a.x;
		x2 = b.x;
		y1 = a.y;
		y2 = b.y;
	}
};

class BOLDescriptor {
private:
	cv::Mat img;
	BOLDFeature* feature;
	std::vector<int> knn_idices;
	std::vector<int> falseLines;
	int cur_idx;
	double getImgPixel(int x, int y);
	BVector getGradient(int x, int y); 
public:
	BOLDescriptor(cv::Mat _img,int idx,std::vector<int> _knn_idices);
	BOLDFeature* getFeature();
	void computeAngle(std::vector<Line> lines,int a, int b);
	void describe(std::vector<Line> lines);
	void showDescriptor();
	int getDescriptorIdx();
};