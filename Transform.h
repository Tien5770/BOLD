#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include"BOLDescriptor.h"
#include"BOLDVector.h"

using namespace std;
using namespace cv;

struct Record {
	double xc;
	double yc;
	double true_x;
	double true_y;
	double scale_fac;
	double rota;
};

const double ransac_thresh = 3.0f;
int A[5][5][5][12];
//hori translation,vertical translation,scale,rotation

template<typename T>
T RandT(T _min, T _max)
{
	T temp;
	if (_min > _max)
	{
		temp = _max;
		_max = _min;
		_min = temp;
	}
	return rand() / (double)RAND_MAX * (_max - _min) + _min;
}

void drawBoundingBox(cv::Mat image, std::vector<cv::Point2f> bb)
{
	for (unsigned i = 0; i < bb.size() - 1; i++)
	{
		line(image, bb[i], bb[i + 1], cv::Scalar(0, 255, 255), 3);

	}
	line(image, bb[bb.size() - 1], bb[0], cv::Scalar(0, 255, 255), 3);
}
void drawGrid(cv::Mat& img, double size)
{
	for (int i = 0; i < img.rows; i+=(int)(size * img.rows))
	{
		cv::line(img, cv::Point(0,i), cv::Point(img.cols,i), cv::Scalar(0, 255, 255), 1);
	}
	for (int i = 0; i < img.cols; i += (int)(size * img.cols))
	{
		cv::line(img, cv::Point(i,0), cv::Point(i,img.rows), cv::Scalar(0, 255, 255), 1);
	}
}
void getHoughTransform(Mat model_img,Mat scene_img,vector<BOLDescriptor> model, vector<BOLDescriptor> scene,
	vector<Line> model_lines,vector<Line>scene_lines,vector<int> matches)
{
	vector<Record> records(scene.size());
	cv::Mat model_img2 = model_img.clone();
	cv::Mat scene_img2 = scene_img.clone();
	cout << "1" << endl;
	drawGrid(scene_img2, 0.25);
	cout << "2" << endl;
	for (int i = 0; i < scene.size(); i++)
	{
		Line scene_tmp = scene_lines[scene[i].getDescriptorIdx()];
		Line model_tmp = model_lines[model[matches[i]].getDescriptorIdx()];

		
		BVector scene_vec2(scene_tmp.x2, scene_tmp.y2, 0);
		BVector scene_vec1(scene_tmp.x1, scene_tmp.y1, 0);
		BVector scene_vec3;
		scene_vec3.set(scene_vec2.minus2D(scene_vec1));
		

		BVector model_vec2(model_tmp.x2, model_tmp.y2, 0);
		BVector model_vec1(model_tmp.x1, model_tmp.y1, 0);
		BVector model_vec3;
		model_vec3.set(model_vec2.minus2D(model_vec1));

		//计算旋转
		double tmp = model_vec3.dot(scene_vec3) / (model_vec3.abs2D() * scene_vec3.abs2D());
		double rota_angle = acos(tmp);

		//计算缩放（长度）
		double model_line_length = model_vec3.abs2D();
		double scene_line_length = scene_vec3.abs2D();
		double scale_factor = (double)model_line_length / scene_line_length;

		//计算中心点位置
		int ref_x = model_img.cols / 2;
		int ref_y = model_img.rows / 2;

		BVector center_x1y1(model_vec1.getElement(0)-ref_x, model_vec1.getElement(1) - ref_y,0);
		double new_c_x = center_x1y1.getElement(0) * cos(rota_angle) - center_x1y1.getElement(1) * sin(rota_angle);
		double new_c_y = center_x1y1.getElement(0) * sin(rota_angle) + center_x1y1.getElement(1) * cos(rota_angle);

		center_x1y1.setElement(0, new_c_x);
		center_x1y1.setElement(1, new_c_y);
		center_x1y1.set(center_x1y1.divByScalar(scale_factor));
		
		double xc = scene_vec1.getElement(0) - center_x1y1.getElement(0);
		double yc = scene_vec1.getElement(1) - center_x1y1.getElement(1);

		rota_angle /= (M_PI / 6);
		
		cv::line(scene_img2, cv::Point(scene_lines[scene[i].getDescriptorIdx()].x1, scene_lines[scene[i].getDescriptorIdx()].y1), cv::Point(scene_lines[scene[i].getDescriptorIdx()].x2, scene_lines[scene[i].getDescriptorIdx()].y2), cv::Scalar(0, 255, 0), 2);
		//cv::circle(scene_img2, cv::Point(model_vec1.getElement(0), model_vec1.getElement(1)), 1, cv::Scalar(255, 255, 0), 4);
		cv::line(model_img2, cv::Point(model_lines[model[matches[i]].getDescriptorIdx()].x1, model_lines[model[matches[i]].getDescriptorIdx()].y1), cv::Point(model_lines[model[matches[i]].getDescriptorIdx()].x2, model_lines[model[matches[i]].getDescriptorIdx()].y2), cv::Scalar(0, 255, 0), 2);
		//cv::circle(model_img2, cv::Point(model_vec1.getElement(0), model_vec1.getElement(1)), 1, cv::Scalar(255, 255, 0), 4);
		//cv::line(scene_img2, cv::Point(model_lines[model[matches[i]].getDescriptorIdx()].x1, model_lines[model[matches[i]].getDescriptorIdx()].y1), cv::Point(model_lines[model[matches[i]].getDescriptorIdx()].x2, model_lines[model[matches[i]].getDescriptorIdx()].y2), cv::Scalar(0, 255, 0), 2);
		cv::circle(scene_img2, cv::Point(xc, yc), 1, cv::Scalar(255, 255, 0), 4);
		records[i].rota = (int)rota_angle;
		records[i].scale_fac = (int)scale_factor;
		records[i].true_x = xc;
		records[i].true_y = yc;

		if(xc <= 0.25 * scene_img.cols)
			xc = 1;
		else if(xc <= 0.5 * scene_img.cols)
			xc = 2;
		else if(xc <= 0.75 * scene_img.cols)
			xc = 3;
		else if(xc <= scene_img.cols)
			xc = 4;

		if (yc <= 0.25 * scene_img.rows)
			yc = 1;
		else if(yc <= 0.5 * scene_img.rows)
			yc = 2;
		else if(yc <= 0.75 * scene_img.rows)
			yc = 3;
		else if(yc <= scene_img.rows)
			yc = 4;
		
		if ((int)rota_angle < 12 && (int)rota_angle >= 0 && (int)scale_factor >= 0 && (int)scale_factor < 5 &&
			(int)xc >= 0 && (int)xc <= 4 && (int)yc >= 0 && (int)yc <= 4)
		{
			A[(int)xc][(int)yc][(int)scale_factor][(int)rota_angle]++;
		}
		records[i].xc = (int)xc;
		records[i].yc = (int)yc;
	}

	int maxCount = -1;
	int max_scale = -1;
	int max_rota = -1;
	int max_xc = -1;
	int max_yc = -1;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 12; j++)
				{
					if (A[x][y][i][j] > maxCount)
					{
						maxCount = A[x][y][i][j];
						max_scale = i;
						max_rota = j;
						max_xc = x;
						max_yc = y;
					}
				}
				
			}
		}
	}
	
	cv::imshow("scene", scene_img2);
	cv::imshow("model", model_img2);
	cv::waitKey(0);
	vector<Point2f> model_points, scene_points;

	double sumX = 0;
	double sumY = 0;
	for (int i = 0;i < records.size(); i++)
	{
		if (records[i].rota == max_rota && records[i].scale_fac == max_scale && records[i].xc==max_xc && records[i].yc == max_yc)
		{
			int r = RandT(0, 255);
			int g = RandT(0, 255);
			int b = RandT(0, 255);
			cv::line(scene_img, cv::Point(scene_lines[scene[i].getDescriptorIdx()].x1, scene_lines[scene[i].getDescriptorIdx()].y1), cv::Point(scene_lines[scene[i].getDescriptorIdx()].x2, scene_lines[scene[i].getDescriptorIdx()].y2), cv::Scalar(r, g, b), 2);
			cv::line(model_img, cv::Point(model_lines[model[matches[i]].getDescriptorIdx()].x1, model_lines[model[matches[i]].getDescriptorIdx()].y1), cv::Point(model_lines[model[matches[i]].getDescriptorIdx()].x2, model_lines[model[matches[i]].getDescriptorIdx()].y2), cv::Scalar(r, g, b), 2);
			sumX += records[i].true_x;
			sumY += records[i].true_y;
			model_points.push_back(Point2f(model_lines[model[matches[i]].getDescriptorIdx()].x1, model_lines[model[matches[i]].getDescriptorIdx()].y1));
			model_points.push_back(Point2f(model_lines[model[matches[i]].getDescriptorIdx()].x2, model_lines[model[matches[i]].getDescriptorIdx()].y2));
			scene_points.push_back(Point2f(scene_lines[scene[i].getDescriptorIdx()].x1, scene_lines[scene[i].getDescriptorIdx()].y1));
			scene_points.push_back(Point2f(scene_lines[scene[i].getDescriptorIdx()].x2, scene_lines[scene[i].getDescriptorIdx()].y2));
			//cv::imshow("scene", scene_img);
			//cv::imshow("model", model_img);
			//cv::waitKey(0);
		}

	}
	
	cv::circle(scene_img, cv::Point2f(sumX / maxCount, sumY / maxCount), 1, cv::Scalar(255, 255, 0), 5);
	/*
	cv::Mat homo;
	if (model_points.size() >= 4)
	{
		homo = cv::findHomography(model_points, scene_points,
			cv::LMEDS, ransac_thresh);
	}
	if (model_points.size() < 4 || homo.empty())
	{//匹配效果不好,未能求解到单应矩阵
		cerr << "Less than 4 matches to find homo" << endl;
		return;
	}
	std::vector<cv::Point2f> obj_bb;
	obj_bb.push_back(Point2f(0, 0));
	obj_bb.push_back(Point2f(model_img.cols, 0));
	obj_bb.push_back(Point2f(model_img.cols, model_img.rows));
	obj_bb.push_back(Point2f(0, model_img.rows));
	std::vector<cv::Point2f> new_bb;
	while (!new_bb.empty()) new_bb.pop_back();
	cv::perspectiveTransform(obj_bb, new_bb, homo);

	drawBoundingBox(scene_img, new_bb);
	*/
	cv::imshow("scene", scene_img); 
	cv::imshow("model", model_img);
	cv::waitKey(0);
}