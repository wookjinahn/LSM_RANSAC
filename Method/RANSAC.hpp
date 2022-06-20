//
// Created by wj on 22. 6. 20.
//

#ifndef LSM_RANSAC_RANSAC_HPP
#define LSM_RANSAC_RANSAC_HPP

#include <iostream>
#include <random>
#include "../Model/Linear.hpp"
#include "../Model/Plane.hpp"

namespace camel
{
	template<typename MODEL>
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
	public:

	private:
		MODEL mModel;

		int mMaxIteration = 0;
		std::vector<float> mBestModelParams;
		int mInlierNum = 0;
	};
}

#endif //LSM_RANSAC_RANSAC_HPP
