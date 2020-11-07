#include"BOLDescriptor.h"

class BOLD {
private:
	cv::Mat img, grayImg;
	std::vector<BOLDescriptor> descriptors;
	std::vector<Line> lines;//检测到图像中的直线
	std::vector<std::vector<int> > lines_knn;
public:
	BOLD(const cv::Mat _img);
	std::vector<int> kNearestNeighbor(int idx);
	void setImg(std::string img_name);
	void showLines();
	void showSingleLine(int idx);
	void showNLines(int idx);
	void detectLines();
	void getDescriptors();
	void showALLDes();
	void printKNN()
	{
		for (int i = 0; i < lines_knn.size(); i++)
		{
			for (int j = 0; j < lines_knn[i].size(); j++)
			{
				std::cout << lines_knn[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	std::vector<Line> getLines()
	{
		return lines;
	}
	std::vector<BOLDescriptor> retDescriptors();
};