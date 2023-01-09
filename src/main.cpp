#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG 
#pragma comment(lib, "opencv_world470d.lib")
#else 
#pragma comment(lib, "opencv_world470.lib")
#endif
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#define IMAGE_NUM 13

struct size
{
	int height;
	int width;
};

void quit() {
	system("pause");
	exit(EXIT_FAILURE);
}

double rmse(cv::Mat& m0, cv::Mat& m1) {
	double res = 0;
	int H = m0.rows, W = m0.cols, blanks = 0;
#pragma omp parallel for
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++) {
			cv::Vec3b p0 = m0.at<cv::Vec3b>(i, j);
			cv::Vec3b p1 = m1.at<cv::Vec3b>(i, j);
			double diff_r = abs(p0.val[0] - p1.val[0]);
			double diff_g = abs(p0.val[1] - p1.val[1]);
			double diff_b = abs(p0.val[2] - p1.val[2]);
			// normalize 256 gradation
			res += sqrt(diff_r * diff_r + diff_g * diff_g + diff_b * diff_b) / 255.0 / sqrt(3.0);
		}
	// calculate rmse per pixel
	res /= H * W - blanks;
	return res;
}

int main()
{
	// open output file
	std::string txtFile = "output.txt";
	FILE* file = fopen(txtFile.c_str(), "w");

	// prepare ideal image
	std::string ideal = "../images/ideal.jpg";
	cv::Mat m_ideal = cv::imread(ideal);
	if (m_ideal.empty()) {
		printf("Error: Cannot read file %s", ideal.c_str());
		quit();
	}
	ideal = ideal.substr(0, ideal.find_last_of('.'));
	size s_ideal;
	s_ideal.height = m_ideal.rows;
	s_ideal.width = m_ideal.cols;

	// prepare compared images
	std::vector<std::string> images;
	for (int i = 16; i <= 16 * IMAGE_NUM; i = i + 16)
	{
		std::string filepath = "../images/" + std::to_string(i) + ".jpg";
		images.push_back(filepath);
	}

	for (int i = 0; i < IMAGE_NUM; i++)
	{
		std::string input = images[i];
		cv::Mat m_input = cv::imread(input);
		if (m_input.empty()) {
			printf("Error: Cannot read file %s", input.c_str());
			quit();
		}

		input = input.substr(0, input.find_last_of('.'));
		size s_input;
		s_input.height = m_input.rows;
		s_input.width = m_input.cols;

		if (s_ideal.height != s_input.height || s_ideal.width != s_input.width)
		{
			printf("Error: Images are of different dimensions.\n");
			quit();
		}

		double rmse_val = rmse(m_ideal, m_input);
		fprintf(file, "sample:%i RMSE:%.4f\n", i * 16 + 16, rmse_val * 100);
	}
	return 0;
}