#pragma once
#include<vector>
#include"BOLDescriptor.h"
using namespace std;

vector<int> Match(vector<BOLDescriptor> A, vector<BOLDescriptor> B)
{//A³¡¾°BÄ£°å
	cout << A.size() <<" "<< B.size() << endl;
	vector<int> res(A.size());
	for (int i = 0; i < A.size(); i++)
	{
		int min_idx = -1;
		double min_dis = INT_MAX;
		BOLDFeature* tmp = A[i].getFeature();
		for (int j = 0; j < B.size(); j++)
		{
			double dis = tmp->euclidDistanceFrom(B[j].getFeature());
			
			if (dis < min_dis)
			{
				min_idx = j;
				min_dis = dis;
			}
		}
		if(min_idx!=-1)res[i] = min_idx;
		else
		{
			res[i] = 0;
			cout << "EEEEE" << endl;
		}
	}
	return res;
}