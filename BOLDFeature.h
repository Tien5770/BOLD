#pragma once
#include<vector>
const int BINS = 12;
#define M_PI 3.14159265358979323846
class BOLDFeature {
private:
	double histogram[BINS][BINS]; //12*12的描述子直方图
	int entries;
	double binSize;
	int maxCount;
public:
	//构造函数
	BOLDFeature();
	//直方图插入
	void add(double alpha, double beta);
	void normalize(); 
	//欧氏距离计算
	double euclidDistanceFrom(BOLDFeature* f);
	double diffFrom(BOLDFeature* f);
	double getElement(int i, int j);
	void showHist();
	

};