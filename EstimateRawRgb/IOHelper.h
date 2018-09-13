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

	template<typename T> static void ReadCameraResponseFileTovector(std::string file, std::vector<T>& rawRgbR, std::vector<T>&  rawRgbG, std::vector<T>&  rawRgbB, std::vector<T>& srgbR, std::vector<T>&  srgbG, std::vector<T>&  srgbB, ReadingOption op = Original )
	{

			switch (op)
			{
			case Original:
				ReadOriginalValues(file, rawRgbR, rawRgbG, rawRgbB, srgbR, srgbG, srgbB);
				break;
			case logarithm:
				ReadLogValues(file, rawRgbR, rawRgbG, rawRgbB, srgbR, srgbG, srgbB);
				break;
			default:
				ReadOriginalValues(file, rawRgbR, rawRgbG, rawRgbB, srgbR, srgbG, srgbB);
				break;
			}

	}

	template<typename T> static void ReadLogValues(std::string file, std::vector<T>& rawRgbR, std::vector<T>&  rawRgbG, std::vector<T>&  rawRgbB, std::vector<T>& srgbR, std::vector<T>&  srgbG, std::vector<T>&  srgbB)
	{
		fstream fs;
		fs.open(file);
		double r, g, b, sr, sg, sb;

		if (fs.is_open())
		{
			double r, g, b, sr, sg, sb;
			while (fs >> r >> g >> b >> sr >> sg >> sb)
			{
				rawRgbR.push_back(log(r));
				rawRgbG.push_back(log(g));
				rawRgbB.push_back(log(b));
				srgbR.push_back(log(sr));
				srgbG.push_back(log(sg));
				srgbB.push_back(log(sb));
			}
		}
		fs.close();

	}
	template<typename T> static void ReadOriginalValues(std::string file, std::vector<T>& rawRgbR, std::vector<T>&  rawRgbG, std::vector<T>&  rawRgbB, std::vector<T>& srgbR, std::vector<T>&  srgbG, std::vector<T>&  srgbB)
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

