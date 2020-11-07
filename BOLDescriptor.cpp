#pragma once
#include"BOLDescriptor.h"

BOLDescriptor::BOLDescriptor(cv::Mat _img,int _idx,std::vector<int> _knn_idices)
{
	if (_img.type() == CV_8UC1)
	{
		img = _img.clone();
	}
	else
	{
		std::cerr << "BOLD::BOLDescriptor::BOLDescriptor(): Warning! Wrong image type with three channels! \n";
	}
	cur_idx = _idx;
	knn_idices = _knn_idices;
}

void BOLDescriptor::describe(std::vector<Line> lines)
{
	feature = new BOLDFeature();
	int nLines = lines.size();
	for (int i = 0; i < K_NEAREST_NUM; i++)
	{
		
		if (knn_idices[i] < nLines && knn_idices[i]>=0)
		{
			computeAngle(lines,cur_idx, knn_idices[i]);
		}
	}
	feature->normalize();
}
auto checkFalseLine(std::vector<int> v, int index)->bool 
{
	for (int i = 0; i < v.size(); i++) {
		if (index == v.at(i))
			return true;
	}
	return false;
}
void BOLDescriptor::computeAngle(std::vector<Line> lines,int i, int j)
{
	if (i == j) return;
	double alpha, beta, sign;
	BVector gm_i, gm_j;
	BVector s_i, s_j, t_ij, t_ji, n(0, 0, 1), st;
	
	gm_i.set(getGradient((int)((lines[i].x1 + lines[i].x2)) / 2, (int)((lines[i].y1 + lines[i].y2) / 2)));
	gm_j.set(getGradient((int)((lines[j].x1 + lines[j].x2)) / 2, (int)((lines[j].y1 + lines[j].y2) / 2)));
	if (gm_i.abs() == 0)
	{
		std::cerr << "BOLD::BOLDescriptor::describe(): Warning! mid-line gradient was 0. Line omitted! \n";
		return;
	}
	if (gm_j.abs() == 0)
	{
		std::cerr << "BOLD::BOLDescriptor::describe(): Warning! mid-line gradient was 0. Line omitted! \n";
		return;
	}
	BVector e_i1, e_i2, e_j1, e_j2, m_i, m_j;
	m_i.set((lines[i].x1 + lines[i].x2) / 2, (lines[i].y1 + lines[i].y2) / 2, 0);
	m_j.set((lines[j].x1 + lines[j].x2) / 2, (lines[j].y1 + lines[j].y2) / 2, 0);
	t_ij.set(m_j.minus2D(m_i));//tij = tj - ti
	t_ji.set(m_i.minus2D(m_j));//tji = tj - ti

	e_i1.set(lines[i].x1, lines[i].y1, 0);
	e_i2.set(lines[i].x2, lines[i].y2, 0);
	e_j1.set(lines[j].x1, lines[j].y1, 0);
	e_j2.set(lines[j].x2, lines[j].y2, 0);

	//计算sign确定canonical orientation
	BVector signPart;
	signPart.set(e_i2.minus2D(e_i1).cross(gm_i));
	sign = n.dot(signPart.divByScalar(signPart.abs()));
	if (sign < 0)
	{
		float tmp = lines[i].x1;
		lines[i].x1 = lines[i].x2;
		lines[i].x2 = tmp;
		tmp = lines[i].y1;
		lines[i].y1 = lines[i].y2;
		lines[i].y2 = tmp;
	}
	s_i.set(e_i2.minus2D(e_i1).timesScalar(sign));

	signPart.set(e_j2.minus2D(e_j1).cross(gm_j));
	sign = n.dot(signPart.divByScalar(signPart.abs()));
	if (sign < 0)
	{
		float tmp = lines[j].x1;
		lines[j].x1 = lines[j].x2;
		lines[j].x2 = tmp;
		tmp = lines[j].y1;
		lines[j].y1 = lines[j].y2;
		lines[j].y2 = tmp;
	}
	s_j.set(e_j2.minus2D(e_j1).timesScalar(sign));

	//计算alpha,beta
	double tmp = s_i.dot(t_ij) / (s_i.abs() * t_ij.abs());
	alpha = acos(tmp);
	tmp = s_j.dot(t_ji) / (s_j.abs() * t_ji.abs());
	beta = acos(tmp);

	//disambiguation
	int judge_alpha = (int)(s_i.cross(t_ij).divByScalar(s_i.cross(t_ij).abs()).dot(n));
	int judge_beta = (int)(s_j.cross(t_ji).divByScalar(s_j.cross(t_ji).abs()).dot(n));
	if (judge_alpha != 1) alpha = 2 * M_PI - alpha;
	if (judge_beta != 1)beta = 2 * M_PI - beta;

	if (std::isnan(alpha))
	{
		std::cerr << "BOLD::BOLDescriptor::describe(): Warning! alpha was NaN. Omitting line.\n";
		return;
	}
	if (std::isnan(beta))
	{
		std::cerr << "BOLD::BOLDescriptor::describe(): Warning! beta was NaN. Omitting line.\n";
		return;
	}
	feature->add(alpha, beta);
}

auto BOLDescriptor::getImgPixel(int x, int y)->double
{
	double tmp = (double)img.ptr<uchar>(y)[x];
	return tmp;
}

auto BOLDescriptor::getGradient(int x, int y)->BVector
{
	int imWidth = img.cols;
	int imHeight = img.rows;
	double gx = getImgPixel((x > 0 ? x : 1) - 1, y) - getImgPixel((x <= imWidth - 2 ? x : imWidth - 2) + 1, y);
	double gy = getImgPixel(x, (y > 0 ? y : 1) - 1) - getImgPixel(x, (y <= imHeight - 2 ? y : imHeight - 2) + 1);
	BVector g(gx, gy, 0);
	return g;
}
void BOLDescriptor::showDescriptor()
{
	feature->showHist();
}

auto BOLDescriptor::getFeature()->BOLDFeature*
{
	return feature;
}

int BOLDescriptor::getDescriptorIdx()
{
	return cur_idx;
}