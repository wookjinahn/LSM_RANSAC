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

        void SetMinimumInlierNum(int inilerNum)
        {
            mMinimumInlierNum = inilerNum;
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

			std::cout << "Best model params : ";
			for (int i = 0; i < mBestModelParameters.size(); i++)
			{
				std::cout << mBestModelParameters[i] << " ";
			}
			std::cout << std::endl;

			return true;
		}

        bool bRunMulti()
        {
            int iter = 0;
            while (iter != mMaxIteration)
            {
                std::vector<camelVector> randomPoints = getRandomPoints();
                mModel.FindParametersWithRandom(randomPoints);		    // mModel->mParameters에 들어가 있음.
                int inlierNum = getInlierNum();

//                if (mInlierNum < inliearNum)	// update
//                {
//
//                    mBestModelParameters = mModel.GetParameters();		// get mModel->mParameters
//                    mInlierNum = inliearNum;
//                }
                if (mMinimumInlierNum < inlierNum)
                {
                    std::cout << "inlierNum : " << inlierNum << std::endl;
                    mInlierNum = 0;
                    mBestModelParameters.push_back(mModel.GetParameters()[0]);
                    mBestModelParameters.push_back(mModel.GetParameters()[1]);
                    mBestModelParameters.push_back(mModel.GetParameters()[2]);
                }
                iter++;
            }

            if (mBestModelParameters.empty())
            {
                std::cout << "ransac running fail" << std::endl;
                return false;
            }

//            std::cout << "Best model params : ";
//            for (int i = 0; i < mBestModelParameters.size(); i++)
//            {
//                std::cout << mBestModelParameters[i] << " ";
//            }
//            std::cout << std::endl;

            return true;
        }

        bool bRunUpper()
        {
            int upperPointsNum = 50;

            for (int i = 0; i < upperPointsNum; i++)
            {
                std::vector<camelVector> upperPoints = getUpperPoints(upperPointsNum);      // 가까운 거 50개
                std::cout << "upperPoints : " << upperPoints.size() << std::endl;
                std::vector<camelVector> randomPoints = getRandomPoints(upperPoints);       // 가까운 거 50개 중 랜덤
                std::cout << "randomPoints : " << randomPoints.size() << std::endl;
                for (int i = 0; i < randomPoints.size(); i++)
                {
                    std::cout << randomPoints[i].GetX() << ", " << randomPoints[i].GetY() << ", " << randomPoints[i].GetZ() << std::endl;
                }

                mModel.FindParametersWithRandom(randomPoints);		    // mModel->mParameters에 들어가 있음.
                int inliearNum = getInlierNum(upperPoints);             // 가까운 거 뽑은 50개들 중에서 inlier
                std::cout << "inlierNum : " << inliearNum << std::endl;

                if (mInlierNum < inliearNum)	// update
                {
                    mBestModelParameters = mModel.GetParameters();		// get mModel->mParameters
                    mInlierNum = inliearNum;
                }
            }

            if (mBestModelParameters.empty())
            {
                std::cout << "ransac running fail" << std::endl;
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

		void GetResultMulti()
		{
			std::cout << "GetResult : " << mData.size() << std::endl;
            for (int i = 0; i < mBestModelParameters.size() / 3; i++)
            {
                std::vector<float> parameters = { mBestModelParameters[i * 3 + 0], mBestModelParameters[i * 3 + 1], mBestModelParameters[i * 3 + 2] };
                for (int j = 0; j < mData.size(); j++)
                {
                    if (bIsContained(mData[j], parameters))
                    {
                        mResultData.push_back(mData[j]);
                    }
                }
            }
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

		std::vector<camelVector> GetResultData() const
		{
			return mResultData;
		};

	private:
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
                std::cout << upperPoints[iter].GetX() << ", " << upperPoints[iter].GetY() << ", " << upperPoints[iter].GetZ() << std::endl;
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

        std::vector<camelVector> getUpperPoints(int number) const
        {
            std::vector<camelVector> upperPoints;
            for (int i = 0; i < number; i++)
            {
                upperPoints.push_back(mData[i]);
            }

            return upperPoints;
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
			float distance = std::abs(mBestModelParameters[0] * data.GetX() + mBestModelParameters[1] * data.GetY() - data.GetZ() + mBestModelParameters[2]) / std::sqrt(mBestModelParameters[0] * mBestModelParameters[0] + mBestModelParameters[1] * mBestModelParameters[1] + 1);
			if (distance < mModelThreshold)
			{
				return true;
			}
			return false;
		}
        bool bIsContainedMulit(camelVector& data, std::vector<float>& parameters)
        {
			float distance = std::abs(parameters[0] * data.GetX() + parameters[1] * data.GetY() - data.GetZ() + parameters[2]) / std::sqrt(parameters[0] * parameters[0] + parameters[1] * parameters[1] + 1);
			if (distance < mModelThreshold)
			{
				return true;
			}
			return false;
        }

		Model mModel;
		std::vector<camelVector> mData;
		std::vector<camelVector> mResultData;

		int mMaxIteration = 0;
		float mModelThreshold = 0;
		std::vector<float> mBestModelParameters;
		int mInlierNum = 0;
        int mMinimumInlierNum = 307200;     // 640 * 480
	};
}

#endif //LSM_RANSAC_RANSAC_HPP
