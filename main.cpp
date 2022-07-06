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
	float modelThreshold = 0.005f;
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

	std::cout << "input data : " << data.size() << std::endl;

	camel::RANSAC<Model::Plane, CamelVector::Point3D> ransac(planeModel, data, modelThreshold, maxIteration);

	if(ransac.bRun())
	{
		std::vector<float> result = ransac.GetBestModelParameters();
		ransac.GetResult();

		std::cout << "result : " << ransac.GetResultData().size() << ", ";
		for (int i = 0; i < result.size(); i++)
		{
			std::cout << result[i] << " ";
		}
		std::cout << std::endl;
	}

	std::string outputPath = "/home/wj/Desktop/Data/ransac/output_data/";
	time_t t;
	struct tm* timeinfo;
	time(&t);
	timeinfo = localtime(&t);

	std::string hour, min;

	if (timeinfo->tm_hour < 10) hour = "0" + std::to_string(timeinfo->tm_hour);
	else hour = std::to_string(timeinfo->tm_hour);

	if (timeinfo->tm_min < 10) min = "0" + std::to_string(timeinfo->tm_min);
	else min = std::to_string(timeinfo->tm_min);

	std::string filePath = outputPath + hour + min + ".pcd";

	std::ofstream fout;
	fout.open(filePath);

	fout << "VERSION" << std::endl;
	fout << "FIELDS x y z" << std::endl;
	fout << "SIZE 4 4 4" << std::endl;
	fout << "TYPE F F F" << std::endl;
	fout << "COUNT 1 1 1" << std::endl;
	fout << "WIDTH 1" << std::endl;
	fout << "HEIGHT " << ransac.GetResultData().size() << std::endl;
	fout << "VIEWPOINT 0 0 0 1 0 0 0" << std::endl;
	fout << "POINTS " << ransac.GetResultData().size() << std::endl;
	fout << "DATA ascii" << std::endl;

	for (int i = 0; i < ransac.GetResultData().size(); i++)
	{
		fout << ransac.GetResultData()[i].GetX() << " " << ransac.GetResultData()[i].GetY() << " " << ransac.GetResultData()[i].GetZ() << "\n";
	}

	fout.close();

	const auto stopTime = std::chrono::high_resolution_clock::now();
	const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
	std::cout << "elapsed time : " << elapsedTime.count() << " ms." << std::endl;
}
