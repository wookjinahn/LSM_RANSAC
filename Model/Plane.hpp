//
// Created by wj on 22. 6. 20.
//

#ifndef LSM_RANSAC_PLANE_H
#define LSM_RANSAC_PLANE_H

#include <vector>
#include <camel-euclid/Point3D.h>
#include <opencv2/opencv.hpp>

namespace Model
{
	class Plane
	{
	public:
		Plane(std::vector<CamelVector::Point3D>& data);

		void SetData(std::vector<CamelVector::Point3D>& data);
		std::vector<CamelVector::Point3D> GetData() const;

		std::vector<float> GetParameters() const;

		void FindParametersWithAll();
		void FindParametersWithRandom(std::vector<CamelVector::Point3D>& randomPoints);

	private:
		std::vector<CamelVector::Point3D> mData;
		std::vector<float> mParameters;
	};
}

#endif //LSM_RANSAC_PLANE_H
