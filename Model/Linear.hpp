//
// Created by wj on 22. 6. 20.
//

#ifndef LSM_RANSAC_LINEAR_H
#define LSM_RANSAC_LINEAR_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <camel-euclid/Vector2D.h>

namespace Model
{
	class Linear // y = ax + b
	{
	public:
		Linear(std::vector<CamelVector::Vector2D>& data);

		std::vector<float> GetRandomPoints() const;
		void FindParams();
		std::vector<float> FindParamsWithSample(std::vector<float>& samplePoints);
		bool bIsInThreshold(float x, float y);
//		void GetInlier(std::vector<float>& inputX, std::vector<float>& inputY, std::vector<float>& inliearX, std::vector<float>& inliearY);
		int GetInlierNum();
		void Predict(std::vector<float>& outputX, std::vector<float>& outputY);

		std::vector<CamelVector::Vector2D> GetInputData() const;
		void SetInputX(std::vector<CamelVector::Vector2D> inputData);

		float GetParamsA() const;
		void SetParamsA(float a);
		float GetParamsB() const;
		void SetParamsB(float b);
		void SetParams(std::vector<float> params);
		std::vector<float> GetParams() const;

		float GetModelThreshold() const;
		void SetModelThreshold(float modelThreshold);

	private:
		std::vector<CamelVector::Vector2D> mInputData;

		float mParamsA = 0.0f;
		float mParamsB = 0.0f;
		float mModelThreshold = 0.0f;
	};
};


#endif //LSM_RANSAC_LINEAR_H
