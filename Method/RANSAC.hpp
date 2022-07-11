//
// Created by wj on 22. 6. 20.
//

#ifndef LSM_RANSAC_RANSAC_HPP
#define LSM_RANSAC_RANSAC_HPP

#include <iostream>
#include <random>

#include <camel-euclid/Point3D.h>
#include <camel-euclid/Point2D.h>

#include "../Model/Plane.hpp"

namespace camel
{
	template<typename Model, typename CamelVector>	// model, data type (ex. point3d)
	class RANSAC
	{
		// 1. <Sample> the number of data points required to fit the model
		// 2. <Compute> model parameters using the sampled data points
		// 3. <Score> by the fraction of inliers within a preset threshold of the model
		// 4. repeat until best model (parameters?)
		//
		// s : Number of sampled points (minimum number needed to fit the model
		// e : Outlier ratio ( = # of outliers / # of data points )
		// T : Number of trials (

		// Model은 파라미터를 찾고, 저장하는 용도?
	public:
		RANSAC(Model& model, std::vector<CamelVector>& data, float modelThreshold, float maxIteration)
		: mModel(model)
		, mData(data)
		, mMaxIteration(maxIteration)
		{
			mModel.SetModelThreshold(modelThreshold);
			mModelThreshold = modelThreshold;
		}

		void SetData(std::vector<CamelVector>& data)
		{
			mData = std::move(data);
		}

		std::vector<CamelVector> GetData() const
		{
			return mData;
		}

		std::vector<float> GetBestModelParameters() const
		{
			return mBestModelParameters;
		}

		bool bRun()
		{
			int iter = 0;
			while (iter != mMaxIteration)
			{
				std::vector<CamelVector> randomPoints = getRandomPoints();
				mModel.FindParametersWithRandom(randomPoints);		// mModel->mParameters에 들어가 있음.
				int inliearNum = getInlierNum();

				if (mInlierNum < inliearNum)	// update
				{
					mBestModelParameters = mModel.GetParameters();		// get mModel->mParameters
					mInlierNum = inliearNum;
				}
				iter++;
			}

			if (mBestModelParameters.empty())
			{
				return false;
			}

			std::cout << "Best model params : ";
			for (int i = 0; i < mBestModelParameters.size(); i++)
			{
				std::cout << mBestModelParameters[i] << " ";
			}
			std::cout << std::endl;

			return true;
		}

		void GetResult()
		{
			std::cout << "GetResult : " << mData.size() << std::endl;
			for (int i = 0; i < mData.size(); i++)
			{
				if (bIsContained(mData[i]))
				{
					mResultData.push_back(mData[i]);
				}
			}
		}

		std::vector<CamelVector> GetResultData() const
		{
			return mResultData;
		};

	private:
		std::vector<CamelVector> getRandomPoints() const
		{
			int dataNum = mData.size();
			std::random_device randomDevice;
			std::mt19937 generator(randomDevice());
			std::uniform_int_distribution<> randomSample(0, dataNum);

			std::vector<CamelVector> randomPoints;
			for (int i = 0; i < 3; i++)
			{
				int iter = randomSample(generator);
				randomPoints.push_back(mData[iter]);
			}
			return randomPoints;
		}

		int getInlierNum()
		{
			int inlierNum = 0;
			for (int i = 0; i < mData.size(); i++)
			{
				if (mModel.bIsInThreshold(mData[i]))
				{
					inlierNum++;
				}
			}
			return inlierNum;
		}

		bool bIsContained(CamelVector& data)
		{
			float distance = std::abs(mBestModelParameters[0] * data.GetX() + mBestModelParameters[1] * data.GetY() - data.GetZ() + mBestModelParameters[2]) / std::sqrt(mBestModelParameters[0] * mBestModelParameters[0] + mBestModelParameters[1] * mBestModelParameters[1] + 1);
			if (distance < mModelThreshold)
			{
				return true;
			}
			return false;
		}

		Model mModel;
		std::vector<CamelVector> mData;
		std::vector<CamelVector> mResultData;

		int mMaxIteration = 0;
		float mModelThreshold = 0;
		std::vector<float> mBestModelParameters;
		int mInlierNum = 0;
	};
}

#endif //LSM_RANSAC_RANSAC_HPP
