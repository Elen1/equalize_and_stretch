#include "opencv2/core/core.hpp"
#include  "opencv2/highgui.hpp"
#include <iostream>

cv::Mat stretched(cv::Mat image)
{
	const int Lmax = 255;
	const int Lmin = 0;
	int Imgmax = -1, Imgmin = 257;
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			if (image.at<uchar>(y, x) > Imgmax)
			{
				Imgmax = image.at<uchar>(y, x);
			}
			if (image.at<uchar>(y, x) < Imgmin)
			{
				Imgmin = image.at<uchar>(y, x);
			}
		}
	}
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			image.at<uchar>(y, x) = (image.at<uchar>(y, x) - Imgmin) * ((Lmax - Lmin) / (Imgmax - Imgmin)) - Lmin;
		}
	}
	return image;
}
cv::Mat equalized(cv::Mat image)
{
	int histogram[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			histogram[(int)image.at<uchar>(y, x)]++;



	float probabilities[256];
	int resolution = image.rows * image.cols;
	for (int i = 0; i < 256; i++)
	{
		probabilities[i] = (float)histogram[i] / resolution;
	}




	float cdfmax = -1, cdfmin = 257;
	float cdf[256];
	cdf[0] = probabilities[0];
	for (int i = 1; i<256; i++)
	{
		cdf[i] = probabilities[i] + cdf[i - 1];
	}

	cv::Mat histImage(image.rows, image.cols, CV_8UC1, cv::Scalar(255, 255, 255));

	for (int i = 0; i<image.rows; i++)
	{
		for (int j = 0; j<image.cols; j++)
		{
			histImage.at<uchar>(i,j) = cdf[image.at<uchar>(i,j)] * 255;
		}
	}
	return histImage;
	

}

int main()
{
	cv::Mat image = cv::imread("C:/Users/Helen/Downloads/car.png", cv::IMREAD_GRAYSCALE);

	cv::namedWindow("image_before", cv::WINDOW_NORMAL);
	cv::imshow("image_before", image);

	cv::Mat result = equalized(image);

	cv::namedWindow("image_after_equalizing", cv::WINDOW_NORMAL);
	cv::imshow("image_after_equalizing", result);

	cv::Mat result1 = stretched(image);

	cv::namedWindow("image_after_stretching", cv::WINDOW_NORMAL);
	cv::imshow("image_after_stretching", result1);

	cv::waitKey(0);
	return 0;
}