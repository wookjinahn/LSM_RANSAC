#include <random>
#include <vector>
#include <string>
#include <cmath>

#include <matplotlibcpp.h>

#include <camel-euclid/Vector2D.h>

#include "Method/RANSAC.hpp"
#include "Method/LSM.hpp"
#include "Model/Linear.hpp"
#include "Model/Plane.hpp"

// random sample line
void makeLinearRegressionSample(std::vector<CamelVector::Vector2D>& data)
{
	std::random_device randomDevice;
//	std::mt19937 generator(randomDevice());
	std::mt19937 generator(201824627);
	std::uniform_real_distribution<float> randomSample(0.0f, 5.0f);
	std::uniform_real_distribution<float> randomX(1.0f, 5.0f);
	std::uniform_real_distribution<float> line(-1.0f, 1.0f);

	// randomly spread 50 * 50
	for (int i = 0; i < 50; i++)
	{
		data.push_back(CamelVector::Vector2D(randomSample(generator), randomSample(generator)));
//		x.push_back(randomSample(generator));
//		y.push_back(randomSample(generator));
	}

	// y = ax + b
	for (int i = 0; i < 100; i++)
	{
		float a = 2.0f;
		float b = -2.0;
		float noise = 0.2;
		float randomLine = line(generator);
		float randomX = randomSample(generator) + randomLine;
		float randomY = a * randomX + noise * randomLine + b;
		if (randomX > 0 && randomY > 0 && randomX < 5 && randomY < 5)
		{
			data.push_back(CamelVector::Vector2D(randomX, randomY));
//			x.push_back(randomX);
//			y.push_back(randomY);
		}
	}
	std::cout << "Random Points Num : " << data.size() << std::endl;
}
int main()
{
	std::vector<CamelVector::Vector2D> data;

	makeLinearRegressionSample(data);	// random smaple

	Model::Linear LRModel(data);

	camel::LSM<Model::Linear> lsm(LRModel);

	lsm.FindParams();

	int range = 5000;
	float dt = 0.001;
	std::vector<CamelVector::Vector2D> outputData;

	for (int i = 0; i < range; i++)
	{
	outputX.push_back(dt * i);
	}
	//	lsm.GetModel().PredictOutputY(outputX, outputY);
	lsm.GetOutputByParams(outputX, outputY);
	std::cout << lsm.GetModel().GetParamsA() <<  ", " << lsm.GetModel().GetParamsB() << std::endl;

	// matplot
	matplotlibcpp::scatter(lsm.GetModel().GetInputX(), lsm.GetModel().GetInputY());

	matplotlibcpp::plot(outputX, outputY);
	matplotlibcpp::show();
}
