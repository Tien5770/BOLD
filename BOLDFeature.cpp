#pragma once
#include"BOLDFeature.h"
#include<cmath>
#include<iostream>
#include <iomanip>
BOLDFeature::BOLDFeature()
{
	for (int i = 0; i < BINS; i++)
	{
		for (int j = 0; j < BINS; j++)
		{
			histogram[i][j] = 0;
		}
	}
	binSize = (2 * M_PI) / BINS;
	maxCount = 0;
	entries = 0;
}
void BOLDFeature::normalize()
{
	for (int i = 0; i < BINS; i++)
	{
		for (int j = 0; j < BINS; j++)
		{
			histogram[i][j] /= entries;
		}
	}
}
double BOLDFeature::euclidDistanceFrom(BOLDFeature* f)
{
	double ret = 0;
	for (int i = 0; i < BINS; i++)
	{
		for (int j = 0; j < BINS; j++)
		{
			ret += pow((histogram[i][j] - f->getElement(i, j)), 2);
		}
	}
	return ret;
}
double BOLDFeature::diffFrom(BOLDFeature* f)
{
	double ret = 0;
	for (int i = 0; i < BINS; i++)
	{
		for (int j = 0; j < BINS; j++)
		{
			ret += abs(histogram[i][j] - f->getElement(i, j));
		}
	}
	return ret;
}
double BOLDFeature::getElement(int i, int j)
{
	return histogram[i][j];
}

void BOLDFeature::add(double alpha, double beta)
{
	int a, b;
	while (alpha >= M_PI * 2) 
	{
		alpha -= M_PI * 2;
	}
	while (beta >= M_PI * 2) 
	{
		beta -= M_PI * 2;
	}
	a = (int)(alpha / binSize);
	b = (int)(beta / binSize);

	histogram[a][b]++;
	if (histogram[a][b] > maxCount)
	{
		maxCount = histogram[a][b];
	}	
	entries++;
}
void BOLDFeature::showHist()
{
	for (int i = 0; i < BINS; i++)
	{
		for (int j = 0; j < BINS; j++)
		{
			std::cout << std::setprecision(5) << histogram[i][j] << " ";
		}
		std::cout << std::endl;
	}
}