//
// Created by wj on 22. 6. 20.
//

#ifndef LSM_RANSAC_LSM_HPP
#define LSM_RANSAC_LSM_HPP

#include <vector>
#include "../Model/Plane.hpp"

namespace camel
{
	template<typename MODEL>
	class LSM
	{
	public:
		LSM(MODEL& model)
				: mModel(model)
		{
		}

		void SetInputData(std::vector<float> inputX, std::vector<float> inputY)
		{
			mModel.SetInputX(inputX);
			mModel.SetInputY(inputY);
		}

		void FindParams()
		{
			mModel.FindParams();
		}

		void GetOutputByParams(std::vector<float>& outputX, std::vector<float>& outputY)
		{
			mModel.Predict(outputX, outputY);
		}

		MODEL GetModel() const
		{
			return mModel;
		}


	private:
		MODEL mModel;
	};
}

#endif //LSM_RANSAC_LSM_HPP
