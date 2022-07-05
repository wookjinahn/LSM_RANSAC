#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

#include <matplotlibcpp.h>

#include <camel-euclid/Vector2D.h>

#include "Method/RANSAC.hpp"
#include "Method/LSM.hpp"
#include "Model/Linear.hpp"
#include "Model/Plane.hpp"


// codes


int main()
{
	const auto startTime = std::chrono::high_resolution_clock::now();

	Model::Plane planeModel;
	std::vector<CamelVector::Point3D> data;
	float modelThreshold = 0.002f;
	float maxIteration = 1000;

	std::ifstream fin;
	fin.open("/home/wj/Desktop/Data/ransac/rmd_1000.pcd");
	std::string line;

	if (fin.is_open())
	{
		int num = 1;
		while (!fin.eof())
		{
			getline(fin, line);
			if (num > 10)
			{
				float dataX, dataY, dataZ;
				std::istringstream iss(line);
				iss >> dataX >> dataY >> dataZ;
				CamelVector::Point3D point = { dataX, dataY, dataZ };
				data.push_back(point);
			}
			num++;
		}
	}
	fin.close();

	camel::RANSAC<Model::Plane, CamelVector::Point3D> ransac(planeModel, data, modelThreshold, maxIteration);

	if(ransac.bRun())
	{
		std::vector<float> result = ransac.GetBestModelParameters();
		ransac.GetResult();

		std::cout << "result : ";
		for (int i = 0; i < result.size(); i++)
		{
			std::cout << result[i] << " ";
		}
		std::cout << std::endl;
	}

	const auto stopTime = std::chrono::high_resolution_clock::now();
	const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
	std::cout << "elapsed time : " << elapsedTime.count() << " ms." << std::endl;
}
