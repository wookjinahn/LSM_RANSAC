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
	// 점 3개를 랜덤으로 뽑음.
	// 3개로 plane 방정식을 찾음 -> OpenCV 이용하여 --> opencv 사용하지 않고 plane parameter 찾는 방법은 추후에
	// 전체 point에 대해 for문을 돌면서 찾은 plane에 threshold 범위 내에 들어가는 갯수 check

	public:
		std::vector<float> GetParameters() const;
		void SetModelThreshold(float modelThreshold);

		void FindParametersWithAll(std::vector<CamelVector::Point3D>& data);
		void FindParametersWithRandom(std::vector<CamelVector::Point3D>& randomPoints);

		bool bIsInThreshold(CamelVector::Point3D& data);
	private:
		std::vector<float> mParameters;		// 여러개의 plane이 생길 경우도 생각.

		float mModelThreshold = 0.0f;
	};
}

#endif //LSM_RANSAC_PLANE_H
