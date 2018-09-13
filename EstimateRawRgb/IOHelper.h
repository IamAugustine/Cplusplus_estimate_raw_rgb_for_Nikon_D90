#pragma once
#include <vector>;
#include <iostream>;
#include <string>;
#include <fstream>;
#include <math.h>;
using namespace std;
enum ReadingOption
{
	Original,
	logarithm,

};

namespace IOHelper
{

	template<typename T> static void ReadCameraResponseFileTovector(std::string file, std::vector<T>& rawRgbR, std::vector<T>&  rawRgbG, std::vector<T>&  rawRgbB, std::vector<T>& srgbR, std::vector<T>&  srgbG, std::vector<T>&  srgbB)
	{

		fstream fs;
		fs.open(file);
		double r, g, b, sr, sg, sb;

		if (fs.is_open())
		{
			double r, g, b, sr, sg, sb;
			while (fs >> r >> g >> b >> sr >> sg >> sb)
			{
				rawRgbR.push_back(r);
				rawRgbG.push_back(g);
				rawRgbB.push_back(b);
				srgbR.push_back(sr);
				srgbG.push_back(sg);
				srgbB.push_back(sb);
			}
		}
		fs.close();

	}
};

