#include"BOLD.h"
#include"LSD.h"
#include<algorithm>

BOLD::BOLD(const cv::Mat _img)
{
	if(_img.type() == CV_8UC1)
	{
		grayImg = _img.clone();
		img = _img.clone();
	}
	else
	{
		img = _img.clone();
		cv::cvtColor(img, grayImg, cv::COLOR_RGB2GRAY);
	}
}

void BOLD::setImg(std::string img_name)
{
	img = cv::imread(img_name, cv::IMREAD_COLOR);
	grayImg = cv::imread(img_name, cv::IMREAD_GRAYSCALE);
}

void BOLD::detectLines()
{
	std::vector<cv::Vec4f> tmp = lsdGetLines(grayImg);
	for (int i = 0; i < tmp.size(); i++)
	{
		lines.push_back(Line(tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3]));
	}
	lines_knn.resize(lines.size());
	for (int i = 0; i < lines.size(); i++)
	{
		lines_knn[i].resize(K_NEAREST_NUM);
	}
}
void BOLD::showLines()
{
	cv::Mat tmp = img.clone();
	if (tmp.type() == CV_8UC1)
	{
		cv::cvtColor(tmp, tmp, cv::COLOR_GRAY2RGB);
	}
	for (int i = 0; i < lines.size(); ++i)
	{
		cv::line(tmp, cv::Point(lines[i].x1, lines[i].y1), cv::Point(lines[i].x2, lines[i].y2), cv::Scalar(255, 0, 0), 2);
	}
	cv::namedWindow("LINES");
	cv::imshow("LINES", tmp);
	cv::waitKey(0);
}
void BOLD::showNLines(int idx)
{
	cv::Mat tmp = img.clone();
	if (tmp.type() == CV_8UC1)
	{
		cv::cvtColor(tmp, tmp, cv::COLOR_GRAY2RGB);
	}
	cv::line(tmp, cv::Point(lines[idx].x1, lines[idx].y1), cv::Point(lines[idx].x2, lines[idx].y2), cv::Scalar(0, 0, 255), 2);
	for (int i = 0; i < lines_knn[idx].size(); i++)
	{
		cv::line(tmp, cv::Point(lines[lines_knn[idx][i]].x1, lines[lines_knn[idx][i]].y1), cv::Point(lines[lines_knn[idx][i]].x2, lines[lines_knn[idx][i]].y2), cv::Scalar(0, 255, 0), 1);
	}
	cv::namedWindow("NLINE");
	cv::imshow("NLINE", tmp);
	cv::waitKey(0);
}
void BOLD::showSingleLine(int i)
{
	cv::Mat tmp = img.clone();
	if (tmp.type() == CV_8UC1)
	{
		cv::cvtColor(tmp, tmp, cv::COLOR_GRAY2RGB);
	}

	cv::line(tmp, cv::Point(lines[i].x1, lines[i].y1), cv::Point(lines[i].x2, lines[i].y2), cv::Scalar(255, 0, 0), 1);
	cv::namedWindow("LINE");
	cv::imshow("LINE", tmp);
	cv::waitKey(0);
}
auto BOLD::kNearestNeighbor(int idx)->vector<int>
{
	Line cur = lines[idx];
	double cur_mid_x, cur_mid_y, tmp_mid_x, tmp_mid_y;
	cur_mid_x = (cur.x1 + cur.x2) / 2;
	cur_mid_y = (cur.y1 + cur.y2) / 2;
	int nLines = lines.size();
	double distances[K_NEAREST_NUM];
	vector<int>KNL_indices(K_NEAREST_NUM);
	//≥ı ºªØ
	for (int i = 0; i < K_NEAREST_NUM; i++)
	{
		distances[i] = std::numeric_limits<double>::max();
		KNL_indices[i] = -1;
	}
	for (int i = 0; i < nLines; i++)
	{
		if (i == idx) continue;
		Line tmp = lines[i];
		tmp_mid_x = (tmp.x1 + tmp.x2) / 2;
		tmp_mid_y = (tmp.y1 + tmp.y2) / 2;
		double dis = sqrt((cur_mid_x - tmp_mid_x) * (cur_mid_x - tmp_mid_x) + (cur_mid_y - tmp_mid_y) * (cur_mid_y - tmp_mid_y));
		for (int j = 0; j < K_NEAREST_NUM; j++)
		{
			if (dis <= distances[j])
			{
				int tmp_idx = i;
				for (int k = K_NEAREST_NUM-1; k >=j + 1; k--)
				{
					distances[k] = distances[k - 1];
					KNL_indices[k] = KNL_indices[k - 1];	
				}
				distances[j] = dis;
				KNL_indices[j] = tmp_idx;
				break;
			}
		}
	}
	lines_knn[idx] = KNL_indices;
	return KNL_indices;
}

void BOLD::getDescriptors()
{
	detectLines();
	for (int i = 0; i < lines.size(); i++)
	{
		
		vector<int> knn = kNearestNeighbor(i);
		BOLDescriptor new_bd(grayImg,i,knn);
		new_bd.describe(lines);
		descriptors.push_back(new_bd);
	}
}

void BOLD::showALLDes()
{
	for (int i = 0; i < descriptors.size(); i++)
	{
		descriptors[i].showDescriptor();
	}
}

auto BOLD::retDescriptors()->std::vector<BOLDescriptor>
{
	return descriptors;
}