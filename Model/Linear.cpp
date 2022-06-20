//
// Created by wj on 22. 6. 20.
//

#include <random>
#include <math.h>
#include "Linear.hpp"

namespace Model
{
	Linear::Linear(std::vector<CamelVector::Vector2D>& data)
	{
		mInputData = std::move(data);
	}

	std::vector<float> Linear::GetRandomPoints() const	// 2개 point
	{
		int dataNum = mInputData.size();
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
//		std::mt19937 generator(1234);
		std::uniform_int_distribution<> randomSample(0, dataNum);

		std::vector<float> randomPoints;
		for (int i = 0; i < 2; i++)
		{
			randomPoints.push_back(mInputData[randomSample(generator)].GetX());
			randomPoints.push_back(mInputData[randomSample(generator)].GetY());
		}
		return randomPoints;	// 2개 point push됨. == size() = 4;
	}

	void Linear::FindParams()		// find parameters a, b : inv(X.T @ X) @ X.T @ y
	{
		int pointsNum = mInputData.size();
		cv::Mat leftMat = cv::Mat::ones(pointsNum, 2, CV_32F);
		cv::Mat pinvLeftMat = cv::Mat::zeros(2, pointsNum, CV_32F);

		// make X matrix for LinearRegression.
		for (int i = 0; i < leftMat.rows; i++)
		{
			leftMat.at<float>(i, 0) = mInputData[i].GetX();
		}

		cv::invert(leftMat, pinvLeftMat, cv::DECOMP_SVD); 	// svd for pseudo inverse

		// make Y matrix for LinearRegression.
		cv::Mat rightMat(pointsNum, 1, CV_32F);
		for (int i = 0; i < rightMat.rows; i++)
		{
			rightMat.at<float>(i,0) = mInputData[i].GetY();
		}

		cv::Mat paramsMat = pinvLeftMat * rightMat;
		mParamsA = paramsMat.at<float>(0, 0);
		mParamsB = paramsMat.at<float>(0, 1);
	}

	std::vector<float> Linear::FindParamsWithSample(std::vector<float>& samplePoints)
	{
		float x1 = samplePoints[0];
		float y1 = samplePoints[1];
		float x2 = samplePoints[2];
		float y2 = samplePoints[3];

		cv::Mat xMat = cv::Mat::ones(2, 2, CV_32F);
		cv::Mat pinvXMat = cv::Mat::zeros(2, 2, CV_32F);

		// make X matrix for LinearRegression.
		xMat.at<float>(0, 0) = x1;
		xMat.at<float>(1, 0) = x2;

		cv::invert(xMat, pinvXMat, cv::DECOMP_SVD); 	// svd for pseudo inverse

		// make Y matrix for LinearRegression.
		cv::Mat yMat(2, 1, CV_32F);
		yMat.at<float>(0,0) = y1;
		yMat.at<float>(1,0) = y2;

		cv::Mat paramsMat = pinvXMat * yMat;

		mParamsA = paramsMat.at<float>(0, 0);
		mParamsB = paramsMat.at<float>(0, 1);

		std::vector<float> params = {mParamsA, mParamsB};
		return params;
	}

	bool Linear::bIsInThreshold(float x, float y)
	{
		// parameter setting 이후
		// 점과 직선 사이의 거리
		float distance = std::abs(mParamsA * x - y + mParamsB) / std::sqrt(mParamsA * mParamsA + 1);
		if (distance < mModelThreshold)
		{
			return true;
		}
		return false;
	}

	int Linear::GetInlierNum()
	{
		int inlierNum = 0;
		for (int i = 0; i < mInputX.size(); i++)
		{
			if (bIsInThreshold(mInputX[i], mInputY[i]))
			{
				inlierNum++;
			}
		}
		return inlierNum;
	}

	void Linear::Predict(std::vector<float>& outputX, std::vector<float>& outputY)	// create line
	{
		for (int i = 0; i < outputX.size(); i++)
		{
			float y = mParamsA * outputX[i] + mParamsB;
			outputY.push_back(y);
		}
	}

	std::vector<float> Linear::GetInputX() const
	{
		return mInputX;
	}

	void Linear::SetInputX(std::vector<float> inputX)
	{
		mInputX = std::move(inputX);
	}

	std::vector<float> Linear::GetInputY() const
	{
		return mInputY;
	}

	void Linear::SetInputY(std::vector<float> inputY)
	{
		mInputX = std::move(inputY);
	}

	float Linear::GetParamsA() const
	{
		return mParamsA;
	}

	void Linear::SetParamsA(float a)
	{
		mParamsA = a;
	}

	float Linear::GetParamsB() const
	{
		return mParamsB;
	}

	void Linear::SetParamsB(float b)
	{
		mParamsB = b;
	}

	void Linear::SetParams(std::vector<float> params)
	{
		mParamsA = params[0];
		mParamsB = params[1];
	}

	std::vector<float> Linear::GetParams() const
	{
		std::vector<float> params;
		params.push_back(mParamsA);
		params.push_back(mParamsB);
		return params;
	}

	float Linear::GetModelThreshold() const
	{
		return mModelThreshold;
	}

	void Linear::SetModelThreshold(float modelThreshold)
	{
		mModelThreshold = modelThreshold;
	}
}
