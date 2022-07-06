//
// Created by wj on 22. 6. 20.
//

#include "Plane.hpp"

#include <utility>

namespace Model
{
	std::vector<float> Plane::GetParameters() const
	{
		return mParameters;
	}

	void Plane::SetModelThreshold(float modelThreshold)
	{
		mModelThreshold = modelThreshold;
	}

	void Plane::FindParametersWithAll(std::vector<CamelVector::Point3D>& data) 	// ax + by + c = z
	{
		int pointsNum = data.size();
		cv::Mat leftMat = cv::Mat::ones(pointsNum, 3, CV_32F);
		cv::Mat pinvLeftMat = cv::Mat::zeros(3, pointsNum, CV_32F);

		// make X matrix for LinearRegression.
		for (int i = 0; i < leftMat.rows; i++)
		{
			leftMat.at<float>(i, 0) = data[i].GetX();
			leftMat.at<float>(i, 1) = data[i].GetY();
		}

		cv::invert(leftMat, pinvLeftMat, cv::DECOMP_SVD); 	// svd for pseudo inverse

		// make Y matrix for LinearRegression.
		cv::Mat rightMat(pointsNum, 1, CV_32F);
		for (int i = 0; i < rightMat.rows; i++)
		{
			rightMat.at<float>(i,0) = data[i].GetZ();
		}

		cv::Mat paramsMat = pinvLeftMat * rightMat;

		// mParameters = [a, b, c]
		mParameters.push_back(paramsMat.at<float>(0, 0));
		mParameters.push_back(paramsMat.at<float>(0, 1));
		mParameters.push_back(paramsMat.at<float>(0, 2));
	}

	void Plane::FindParametersWithRandom(std::vector<CamelVector::Point3D>& randomPoints)
	{
		mParameters.clear();

		CamelVector::Point3D point0 = randomPoints[0];
		CamelVector::Point3D point1 = randomPoints[1];
		CamelVector::Point3D point2 = randomPoints[2];

		cv::Mat leftMat = cv::Mat::ones(3, 3, CV_32F);
		cv::Mat pinvLeftMat = cv::Mat::zeros(3, 3, CV_32F);

		leftMat.at<float>(0, 0) = point0.GetX();
		leftMat.at<float>(1, 0) = point1.GetX();
		leftMat.at<float>(2, 0) = point2.GetX();
		leftMat.at<float>(0, 1) = point0.GetY();
		leftMat.at<float>(1, 1) = point1.GetY();
		leftMat.at<float>(2, 1) = point2.GetY();

		cv::invert(leftMat, pinvLeftMat, cv::DECOMP_SVD); 	// svd for pseudo inverse

		// make Y matrix for LinearRegression.
		cv::Mat rightMat(3, 1, CV_32F);
		rightMat.at<float>(0,0) = point0.GetZ();
		rightMat.at<float>(1,0) = point1.GetZ();
		rightMat.at<float>(2,0) = point2.GetZ();

		cv::Mat paramsMat = pinvLeftMat * rightMat;

		mParameters.push_back(paramsMat.at<float>(0, 0));
		mParameters.push_back(paramsMat.at<float>(0, 1));
		mParameters.push_back(paramsMat.at<float>(0, 2));
	}

	bool Plane::bIsInThreshold(CamelVector::Point3D& data)
	{
		float distance = std::abs(mParameters[0] * data.GetX() + mParameters[1] * data.GetY() - data.GetZ() + mParameters[2]) / std::sqrt(mParameters[0] * mParameters[0] + mParameters[1] * mParameters[1] + 1);
		if (distance < mModelThreshold)
		{
			return true;
		}
		return false;
	}
}