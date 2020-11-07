#pragma once
#include<cstring>
#include<string>
#include<ctime>
#include"BOLD.h"
#include"Matcher.h"
#include"Transform.h"
using namespace std;

int main()
{
	srand((unsigned)time(NULL));
	
	string filename = "D-Textureless/test/test028.jpg";
	string model_file = "D-Textureless/train/model_00.png";
	cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
	cv::Mat model = cv::imread(model_file, cv::IMREAD_COLOR);

	BOLD bd(img);
	bd.getDescriptors();
	vector<BOLDescriptor> B = bd.retDescriptors();
	bd.showLines();

	BOLD model_bd(model);
	model_bd.getDescriptors();
	model_bd.showLines();
	vector<BOLDescriptor> A = model_bd.retDescriptors();
	
	vector<int> matching_res = Match(B,A);
	vector<Line> tmp = model_bd.getLines();
	vector<Line> tmp2 = bd.getLines();
	
	for (int i = 0; i < B.size(); i++)
	{
		int r = RandT(0, 255);
		int g = RandT(0, 255);
		int b = RandT(0, 255);

		cv::Mat scene_tmp = img.clone();
		cv::Mat model_tmp = model.clone();
		cv::line(scene_tmp, cv::Point(tmp2[B[i].getDescriptorIdx()].x1, tmp2[B[i].getDescriptorIdx()].y1), cv::Point(tmp2[B[i].getDescriptorIdx()].x2, tmp2[B[i].getDescriptorIdx()].y2), cv::Scalar(255,255,255), 2);
		cv::line(model_tmp, cv::Point(tmp[A[matching_res[i]].getDescriptorIdx()].x1, tmp[A[matching_res[i]].getDescriptorIdx()].y1), cv::Point(tmp[A[matching_res[i]].getDescriptorIdx()].x2, tmp[A[matching_res[i]].getDescriptorIdx()].y2), cv::Scalar(255,0,0), 2);
		
		//cv::imshow("tmp", model_tmp);
		//cv::imshow("tmp2", scene_tmp);
		//cv::waitKey(0);
	}
	//cv::imshow("tmp", model);
	//cv::imshow("tmp2", img);
	//cv::waitKey(0);
	
	getHoughTransform(model, img, A, B, tmp, tmp2, matching_res);
	return 0;
}