//
// Created by wj on 22. 6. 20.
//

#include "Plane.hpp"
#include <iostream>
#include <utility>

namespace Model
{
	Plane::Plane()
	{
	}

	Plane::Plane(std::vector<camelVector::Point3D>& data, std::vector<float>& parameter)
	{
		mData = data;
		mParameters = parameter;
	}

	std::vector<float> Plane::GetParameters() const
	{
		return mParameters;
	}

	void Plane::SetModelThreshold(float modelThreshold)
	{
		mModelThreshold = modelThreshold;
	}


	std::vector<camelVector::Point3D> Plane::GetData() const
	{
		return mData;
	}


//	void Plane::FindParametersWithAll(std::vector<CamelVector::Point3D>& data) 	// ax + by + c = z
//	{
//		int pointsNum = data.size();
//		cv::Mat leftMat = cv::Mat::ones(pointsNum, 3, CV_32F);
//		cv::Mat pinvLeftMat = cv::Mat::zeros(3, pointsNum, CV_32F);
//
//		// make X matrix for LinearRegression.
//		for (int i = 0; i < leftMat.rows; i++)
//		{
//			leftMat.at<float>(i, 0) = data[i].GetX();
//			leftMat.at<float>(i, 1) = data[i].GetY();
//		}
//
//		cv::invert(leftMat, pinvLeftMat, cv::DECOMP_SVD); 	// svd for pseudo inverse
//
//		// make Y matrix for LinearRegression.
//		cv::Mat rightMat(pointsNum, 1, CV_32F);
//		for (int i = 0; i < rightMat.rows; i++)
//		{
//			rightMat.at<float>(i,0) = data[i].GetZ();
//		}
//
//		cv::Mat paramsMat = pinvLeftMat * rightMat;
//
//		// mParameters = [a, b, c]
//		mParameters.push_back(paramsMat.at<float>(0, 0));
//		mParameters.push_back(paramsMat.at<float>(0, 1));
//		mParameters.push_back(paramsMat.at<float>(0, 2));
//	}

	void Plane::FindParametersWithRandom(std::vector<camelVector::Point3D>& randomPoints)
	{
		mParameters.clear();

        float x1 = randomPoints[0].GetX();
        float y1 = randomPoints[0].GetY();
        float z1 = randomPoints[0].GetZ();
        float x2 = randomPoints[1].GetX();
        float y2 = randomPoints[1].GetY();
        float z2 = randomPoints[1].GetZ();
        float x3 = randomPoints[2].GetX();
        float y3 = randomPoints[2].GetY();
        float z3 = randomPoints[2].GetZ();

        float det_A = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 * (y1 * z2 - y2 * z1);

        float adj_A00 = y2 * z3 - y3 * z2;
        float adj_A01 = -(y1 * z3 - y3 * z1);
        float adj_A02 = y1 * z2 - y2 * z1;
        float adj_A10 = -(x2 * z3 - x3 * z2);
        float adj_A11 = x1 * z3 - x3 * z1;
        float adj_A12 = -(x1 * z2 - x2 * z1);
        float adj_A20 = x2 * y3 - x3 * y2;
        float adj_A21 = -(x1 * y3 - x3 * y1);
        float adj_A22 = x1 * y2 - x2 * y1;

        float parameterA = (adj_A00 + adj_A01 + adj_A02) / det_A;
        float parameterB = (adj_A10 + adj_A11 + adj_A12) / det_A;
        float parameterC = (adj_A20 + adj_A21 + adj_A22) / det_A;

//        mParameters.push_back(parameterA);
//        mParameters.push_back(parameterB);
//        mParameters.push_back(parameterC);
		mParameters = { parameterA, parameterB, parameterC };

		//=============================================================================================================================

//        mParameters.clear();
//
//		camelVector::Point3D point0 = randomPoints[0];
//		camelVector::Point3D point1 = randomPoints[1];
//		camelVector::Point3D point2 = randomPoints[2];
//
//		cv::Mat leftMat = cv::Mat::ones(3, 3, CV_32F);
//		cv::Mat pinvLeftMat = cv::Mat::zeros(3, 3, CV_32F);
//
//		leftMat.at<float>(0, 0) = point0.GetX();
//		leftMat.at<float>(1, 0) = point1.GetX();
//		leftMat.at<float>(2, 0) = point2.GetX();
//		leftMat.at<float>(0, 1) = point0.GetY();
//		leftMat.at<float>(1, 1) = point1.GetY();
//		leftMat.at<float>(2, 1) = point2.GetY();
//
//		cv::invert(leftMat, pinvLeftMat, cv::DECOMP_SVD); 	// svd for pseudo inverse
//
//		// make Y matrix for LinearRegression.
//		cv::Mat rightMat(3, 1, CV_32F);
//		rightMat.at<float>(0,0) = point0.GetZ();
//		rightMat.at<float>(1,0) = point1.GetZ();
//		rightMat.at<float>(2,0) = point2.GetZ();
//
//		cv::Mat paramsMat = pinvLeftMat * rightMat;
//
//		mParameters.push_back(paramsMat.at<float>(0, 0));
//		mParameters.push_back(paramsMat.at<float>(0, 1));
//		mParameters.push_back(paramsMat.at<float>(0, 2));
//        std::cout << "FindParametersWithRandom : " << mParameters[0] << ", " << mParameters[1] << ", " << mParameters[2] << std::endl;

		//=============================================================================================================================

//		mParameters.clear();

//        float x1 = randomPoints[0].GetX();
//        float y1 = randomPoints[0].GetY();
//        float z1 = randomPoints[0].GetZ();
//        float x2 = randomPoints[1].GetX();
//        float y2 = randomPoints[1].GetY();
//        float z2 = randomPoints[1].GetZ();
//        float x3 = randomPoints[2].GetX();
//        float y3 = randomPoints[2].GetY();
//        float z3 = randomPoints[2].GetZ();
//
//        float det_A = x1 * (y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2);
//
//        float adj_A00 = y2 * - y3 ;
//        float adj_A01 = -(y1 - y3);
//        float adj_A02 = y1 - y2;
//        float adj_A10 = -(x2 - x3);
//        float adj_A11 = x1 - x3 ;
//        float adj_A12 = -(x1 - x2);
//        float adj_A20 = x2 * y3 - x3 * y2;
//        float adj_A21 = -(x1 * y3 - x3 * y1);
//        float adj_A22 = x1 * y2 - x2 * y1;
//
//        float parameterA = (adj_A00 * z1 + adj_A01 * z2 + adj_A02 * z3)  / det_A;
//        float parameterB = (adj_A10 * z1 + adj_A11 * z2 + adj_A12 * z3)  / det_A;
//        float parameterC = (adj_A20 * z1 + adj_A21 * z2 + adj_A22 * z3)  / det_A;
//
//        mParameters.push_back(parameterA);
//        mParameters.push_back(parameterB);
//        mParameters.push_back(parameterC);
//        std::cout << "FindParametersWithRandom : " << parameterA << ", " << parameterB << ", " << parameterC << std::endl;
	}

	bool Plane::bIsInThreshold(camelVector::Point3D& data)
	{
        float x = data.GetX();
        float y = data.GetY();
        float z = data.GetZ();

		float distance = std::abs(mParameters[0] * x + mParameters[1] * y + mParameters[2] * z - 1) / std::sqrt(mParameters[0] * mParameters[0] + mParameters[1] * mParameters[1] + mParameters[2] * mParameters[2]);

		if (distance < mModelThreshold)
		{
			return true;
		}
		return false;
	}
}