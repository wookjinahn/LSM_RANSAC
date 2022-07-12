#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <sstream>
#include <fstream>
#include <algorithm>

//#include <matplotlibcpp.h>
#include <camel-euclid/Vector.h>

#include "Method/RANSAC.hpp"
#include "Method/LSM.hpp"
#include "Model/Plane.hpp"



void fromPCD(std::vector<camelVector::Point3D>& data)
{
    std::ifstream fin;
    fin.open("/home/wj/Desktop/Data/palletizeBox.pcd");
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
                camelVector::Point3D point = { dataX, dataY, dataZ };
                data.push_back(point);
            }
            num++;
        }
    }
    fin.close();
}

void toPCD(camel::RANSAC<Model::Plane, camelVector::Point3D>& data)
//void toPCD(std::vector<camelVector::Point3D>& data)
{
    std::cout << "output : " << data.GetResultData().size() << std::endl;
//    std::cout << "output : " << data.size() << std::endl;
    std::string outputPath = "/home/wj/Desktop/Data/output_data/";
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
    fout << "HEIGHT " << data.GetResultData().size() << std::endl;
//    fout << "HEIGHT " << data.size() << std::endl;
    fout << "VIEWPOINT 0 0 0 1 0 0 0" << std::endl;
    fout << "POINTS " << data.GetResultData().size() << std::endl;
//    fout << "POINTS " << data.size() << std::endl;
    fout << "DATA ascii" << std::endl;

    for (int i = 0; i < data.GetResultData().size(); i++)
    {
        fout << data.GetResultData()[i].GetX() << " " << data.GetResultData()[i].GetY() << " " << data.GetResultData()[i].GetZ() << "\n";
    }
//    for (int i = 0; i < data.size(); i++)
//    {
//        fout << data[i].GetX() << " " << data[i].GetY() << " " << data[i].GetZ() << "\n";
//    }

    fout.close();
}

//void toPCD(camel::RANSAC<Model::Plane, camelVector::Point3D>& data)
void toPCD(std::vector<camelVector::Point3D>& data)
{
//    std::cout << "output : " << data.GetResultData().size() << std::endl;
    std::cout << "output : " << data.size() << std::endl;
    std::string outputPath = "/home/wj/Desktop/Data/output_data/";
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
//    fout << "HEIGHT " << data.GetResultData().size() << std::endl;
    fout << "HEIGHT " << data.size() << std::endl;
    fout << "VIEWPOINT 0 0 0 1 0 0 0" << std::endl;
//    fout << "POINTS " << data.GetResultData().size() << std::endl;
    fout << "POINTS " << data.size() << std::endl;
    fout << "DATA ascii" << std::endl;

//    for (int i = 0; i < data.GetResultData().size(); i++)
//    {
//        fout << data.GetResultData()[i].GetX() << " " << data.GetResultData()[i].GetY() << " " << data.GetResultData()[i].GetZ() << "\n";
//    }
    for (int i = 0; i < data.size(); i++)
    {
        fout << data[i].GetX() << " " << data[i].GetY() << " " << data[i].GetZ() << "\n";
    }

    fout.close();
}

int main()
{

    const auto startTime = std::chrono::high_resolution_clock::now();

    Model::Plane planeModel;
    std::vector<camelVector::Point3D> data;
    float modelThreshold = 0.01f;
    int maxIteration = 2047;

    fromPCD(data);

    std::cout << "input data : " << data.size() << std::endl;

    std::sort(data.begin(), data.end(), camelVector::Point3D::AscendingByY);
//    toPCD(data);

    camel::RANSAC<Model::Plane, camelVector::Point3D> ransac(planeModel, data, modelThreshold, maxIteration);
    ransac.SetMinimumInlierNum(500);

    if(ransac.bRunUpper())
    {
        std::vector<float> result = ransac.GetBestModelParameters();
        std::cout << result.size() << std::endl;

        ransac.GetResult();
    }
    toPCD(ransac);

//	if(ransac.bRun())
//	{
//		std::vector<float> result = ransac.GetBestModelParameters();
//		ransac.GetResult();
//
//		std::cout << "result : " << ransac.GetResultData().size() << ", ";
//		for (int i = 0; i < result.size(); i++)
//		{
//			std::cout << result[i] << " ";
//		}
//		std::cout << std::endl;
//	}
//    toPCD(ransac);

	const auto stopTime = std::chrono::high_resolution_clock::now();
	const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
	std::cout << "elapsed time : " << elapsedTime.count() << " ms." << std::endl;
}
