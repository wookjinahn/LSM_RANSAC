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

void toPCD(Model::Plane& model, int number)
{
	std::cout << "output : " << model.GetData().size() << std::endl;
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

	std::string filePath = outputPath + hour + min + + "_" + std::to_string(number) + ".pcd";

	std::ofstream fout;
	fout.open(filePath);

	fout << "VERSION" << std::endl;
	fout << "FIELDS x y z" << std::endl;
	fout << "SIZE 4 4 4" << std::endl;
	fout << "TYPE F F F" << std::endl;
	fout << "COUNT 1 1 1" << std::endl;
	fout << "WIDTH 1" << std::endl;
	fout << "HEIGHT " << model.GetData().size() << std::endl;
	fout << "VIEWPOINT 0 0 0 1 0 0 0" << std::endl;
	fout << "POINTS " << model.GetData().size() << std::endl;
	fout << "DATA ascii" << std::endl;

	for (int i = 0; i < model.GetData().size(); i++)
	{
		fout << model.GetData()[i].GetX() << " " << model.GetData()[i].GetY() << " " << model.GetData()[i].GetZ() << "\n";
	}

	fout.close();
}

int main()
{
    const auto startTime = std::chrono::high_resolution_clock::now();

//	std::vector<int> test = { 11, 22, 33, 44, 55, 66, 77, 88 };
//	for (int i = 0; i < test.size(); i++)
//	{
//		if (test[i] == 33 || test[i] == 77)
//		{
//			test.erase(test.begin() + i);
//		}
//	}
//	for (int i = 0; i < test.size(); i++)
//	{
//		std::cout << test[i] << " ";
//	}
//	std::cout << std::endl;
//
//
    Model::Plane planeModel;
    std::vector<camelVector::Point3D> data;
	fromPCD(data);
	// data sort
	std::sort(data.begin(), data.end(), camelVector::Point3D::AscendingByY);
	std::cout << "input data : " << data.size() << std::endl;

    float modelThreshold = 0.01f;
    int maxIteration = data.size();

    camel::RANSAC<Model::Plane, camelVector::Point3D> ransac(planeModel, data, modelThreshold, maxIteration);

	ransac.RunUpper();

	std::cout << "result data : " << ransac.GetResultModel().size() << std::endl;

	toPCD(ransac.GetResultModel()[0], 0);
	toPCD(ransac.GetResultModel()[1], 1);
	toPCD(ransac.GetResultModel()[2], 2);
	toPCD(ransac.GetResultModel()[3], 3);

	// ============================================================================================================

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
	const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
	std::cout << "elapsed time : " << elapsedTime.count() << " us." << std::endl;
}
