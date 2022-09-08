//
// Created by wj on 22. 6. 20.
//

#ifndef LSM_RANSAC_RANSAC_HPP
#define LSM_RANSAC_RANSAC_HPP

#include <iostream>
#include <random>

#include <camel-euclid/Point3D.hpp>
#include <camel-euclid/Point2D.hpp>

#include "../Model/Plane.hpp"

namespace camel
{
	template<typename Model, typename camelVector>	// model, data type (ex. point3d)
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
		RANSAC(Model& model, std::vector<camelVector>& data, float modelThreshold, int maxIteration)
		: mModel(model)
		, mData(data)
		, mMaxIteration(maxIteration)
		{
			mModel.SetModelThreshold(modelThreshold);
			mModelThreshold = modelThreshold;
		}

		void SetData(std::vector<camelVector>& data)
		{
			mData = std::move(data);
		}

		std::vector<camelVector> GetData() const
		{
			return mData;
		}

		std::vector<float> GetBestModelParameters() const
		{
			return mBestModelParameters;
		}

		std::vector<Model> GetResultModel() const
		{
			return mResultModel;
		}

		bool bRun()
		{
			int iter = 0;
			while (iter != mMaxIteration)
			{
				std::vector<camelVector> randomPoints = getRandomPoints();
				mModel.FindParametersWithRandom(randomPoints);		    // mModel->mParameters에 들어가 있음.
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

			return true;
		}

        void RunUpper()
        {
			int iter = 0;
			int upperPointsNum = 100;
			int getrandomOfUpper = 50;
			while (iter < mMaxIteration)
			{
				std::vector<camelVector> upperPoints = getUpperPoints(upperPointsNum);
				// for shuffle
				auto rng = std::default_random_engine {};
				std::shuffle(std::begin(upperPoints), std::end(upperPoints), rng);

				mBestModelParameters.clear();
				mResultData.clear();
				mInlierNum = 0;
				for (int i = 0; i < getrandomOfUpper; i++)
				{
					std::vector<camelVector> randomPoints = getRandomPoints(upperPoints);       // 가까운 거 50개 중 랜덤

					mModel.FindParametersWithRandom(randomPoints);            // mModel->mParameters에 들어가 있음.
					int inlierNum = getInlierNum(upperPoints);             // 가까운 거 뽑은 50개들 중에서 inlier

					if (mInlierNum < inlierNum)    // update -> 가장 inlier 많은 파라미터 get
					{
						mBestModelParameters = mModel.GetParameters();        // get mModel->mParameters
						mInlierNum = inlierNum;
					}
				}
				std::cout << "inlierNum : " << mInlierNum << std::endl;
				if (!mBestModelParameters.empty())
				{
					GetResultMulti();
					if (!mResultData.empty())
					{
						Model bestModel(mResultData, mBestModelParameters);
						mResultModel.push_back(bestModel);
					}
				}

				iter += getrandomOfUpper;
			}
        }

		void GetResultMulti()
		{
			for (int i = 0; i < mData.size(); i++)
			{
				if (bIsContained(mData[i]))
				{
					mResultData.push_back(mData[i]);
					mData.erase(mData.begin() + i);
					i--;
				}
			}
		}

        void GetResult()
        {
//            std::cout << "GetResult : " << mData.size() << std::endl;
            for (int i = 0; i < mData.size(); i++)
            {
                if (bIsContained(mData[i]))
                {
                    mResultData.push_back(mData[i]);
                }
            }
        }

		std::vector<camelVector> GetResultData() const
		{
			return mResultData;
		};

	private:
		std::vector<camelVector> getUpperPoints(int number) const
		{
			std::vector<camelVector> upperPoints;
			for (int i = 0; i < number; i++)
			{
				upperPoints.push_back(mData[i]);
			}

			return upperPoints;
		}

		std::vector<camelVector> getRandomPoints(std::vector<camelVector>& upperPoints) const
		{
			int dataNum = upperPoints.size();
			std::random_device randomDevice;
			std::mt19937 generator(randomDevice());
			std::uniform_int_distribution<> randomSample(0, dataNum);

			std::vector<camelVector> randomPoints;
			for (int i = 0; i < 3; i++)
			{
				int iter = randomSample(generator);
				randomPoints.push_back(upperPoints[iter]);
			}

			return randomPoints;
		}

//        std::vector<camelVector> getRandomPoints(int upperNum) const
//        {
//            std::random_device randomDevice;
//            std::mt19937 generator(randomDevice());
//            std::uniform_int_distribution<> randomSample(0, upperNum);
//
//            std::vector<camelVector> randomPoints;
//            for (int i = 0; i < 3; i++)
//            {
//                int iter = randomSample(generator);
//                randomPoints.push_back(mData[iter]);
//            }
//            return randomPoints;
//        }
//
//        std::vector<camelVector> getRandomPoints() const
//        {
//            int dataNum = mData.size();
//            std::random_device randomDevice;
//            std::mt19937 generator(randomDevice());
//            std::uniform_int_distribution<> randomSample(0, dataNum);
//
//            std::vector<camelVector> randomPoints;
//            for (int i = 0; i < 3; i++)
//            {
//                int iter = randomSample(generator);
//                randomPoints.push_back(mData[iter]);
//            }
//            return randomPoints;
//        }


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

        int getInlierNum(std::vector<camelVector>& upperPoints)
        {
            int inlierNum = 0;
            for (int i = 0; i < upperPoints.size(); i++)
            {
                if (mModel.bIsInThreshold(upperPoints[i]))
                {
                    inlierNum++;
                }
            }
            return inlierNum;
        }

		bool bIsContained(camelVector& data)
		{
			float x = data.GetX();
			float y = data.GetY();
			float z = data.GetZ();

			float distance = std::abs(mBestModelParameters[0] * x + mBestModelParameters[1] * y + mBestModelParameters[2] * z - 1) / std::sqrt(mBestModelParameters[0] * mBestModelParameters[0] + mBestModelParameters[1] * mBestModelParameters[1] + mBestModelParameters[2] * mBestModelParameters[2]);

			if (distance < mModelThreshold)
			{
				return true;
			}
			return false;
		}
        bool bIsContainedMulit(camelVector& data, std::vector<float>& parameters)
        {
			float x = data.GetX();
			float y = data.GetY();
			float z = data.GetZ();

			float distance = std::abs(mBestModelParameters[0] * x + mBestModelParameters[1] * y + mBestModelParameters[2] * z - 1) / std::sqrt(mBestModelParameters[0] * mBestModelParameters[0] + mBestModelParameters[1] * mBestModelParameters[1] + mBestModelParameters[2] * mBestModelParameters[2]);

			if (distance < mModelThreshold)
			{
				return true;
			}
			return false;
        }

		Model mModel;
		std::vector<camelVector> mData;
		std::vector<camelVector> mResultData;
		std::vector<Model> mResultModel;

		int mMaxIteration = 0;
		float mModelThreshold = 0;
		std::vector<float> mBestModelParameters;
		int mInlierNum = 0;
	};
}

#endif //LSM_RANSAC_RANSAC_HPP
