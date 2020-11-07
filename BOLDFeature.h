#pragma once
#include<vector>
const int BINS = 12;
#define M_PI 3.14159265358979323846
class BOLDFeature {
private:
	double histogram[BINS][BINS]; //12*12��������ֱ��ͼ
	int entries;
	double binSize;
	int maxCount;
public:
	//���캯��
	BOLDFeature();
	//ֱ��ͼ����
	void add(double alpha, double beta);
	void normalize(); 
	//ŷ�Ͼ������
	double euclidDistanceFrom(BOLDFeature* f);
	double diffFrom(BOLDFeature* f);
	double getElement(int i, int j);
	void showHist();
	

};