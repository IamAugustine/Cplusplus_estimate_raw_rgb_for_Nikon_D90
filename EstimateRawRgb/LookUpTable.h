#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
enum InterpolationType
{
	SegmentLinear,
	Lagrange,
	Spline3,
};
class Lut1DInterpolation
{
public:
	Lut1DInterpolation();
	template<typename TInput, typename TValue> 
	Lut1DInterpolation(vector<TInput> &input, vector<TValue> &values)
	{
		vector<TInput>::iterator itInput = input.begin();
		vector<TValue>::iterator itValue = values.begin();
		while (itInput != input.end() && itValue != values.end())
		{
			DataTable.insert(std::pair<TInput, TValue>(*itInput, *itValue));
			itInput++;
			itValue++;
		}
		ExtendDataTable();
	}
	~Lut1DInterpolation();
public:
	map<double, double> DataTable;
	vector<double> xNode;
	vector<double> yNode;
private:
	map<double, double> FullSrgbDataTable;
	void ExtendDataTable();

public:
	template<typename T> 
	vector<T> GetValue(vector<T> &x, InterpolationType interpType = InterpolationType::SegmentLinear)
	{
		vector<T> results;
		for (T const& v: x)
		{
			results.push_back(GetValue(v, interpType));
		}
		return results;
	}

public:
	template<typename T> 
	T GetValue(T x, InterpolationType type= InterpolationType::SegmentLinear)
	{
		switch (type)
		{
		case SegmentLinear:
			return GetValue_SegmentLinearInterp(x);
			break;
		case Lagrange:
			//return GetValue_Lagrange(x);
			break;
		case Spline3:
			break;
		default:
			break;
		}
	}
private:
	template<typename T> 
	T GetValue_SegmentLinearInterp(T x)
	{
		double upKey, upValue, lowKey, lowValue;
		std::map<double, double>::iterator it_map = DataTable.begin();

		while (it_map != DataTable.end())
		{
			T key = it_map->first;
			if (key > x)
				break;
			it_map++;
		}
		if (it_map == DataTable.begin())
		{
			return	LagrangeExtrapl(x, true);
		}
		else if (it_map == DataTable.end())
		{
			return	LagrangeExtrapl(x, false);
		}
		else
		{
			upKey = it_map->first;
			upValue = it_map->second;
			it_map--;
			lowKey = it_map->first;
			lowValue = it_map->second;
			return LinearInterp<double>(lowKey, lowValue, upKey, upValue, x);
		}
		
	}
	template<typename T> T 
	LinearInterp(T x1, T y1, T x2, T y2, T x)
	{
		return (x - x2)*y1*1.0f / (x1 - x2) + (x - x1)*y2*1.0f / (x2 - x1);
	}
	template<typename T> T
	LagrangeExtrapl(T x, bool isLeftExtrapolation)
	{
		int extraplNodeCount = 4;
		vector<T> vec_extrapl_X;
		vector<T> vec_extrapl_Y;
		if (isLeftExtrapolation)
		{
			map<double,double>::iterator it = DataTable.begin();
			for (int i = 0; i < extraplNodeCount; i++)
			{
				vec_extrapl_X.push_back(it->first);
				vec_extrapl_Y.push_back(it->second);
				it++;
			}
		}
		else
		{
			map<double, double>::reverse_iterator it = DataTable.rbegin();
			for (int i = 0; i < extraplNodeCount; i++)
			{
				vec_extrapl_X.push_back(it->first);
				vec_extrapl_Y.push_back(it->second);
				it++;
			}
		}
		return GetValue_Lagrange(vec_extrapl_X, vec_extrapl_Y, x);
	}
	template<typename T> T 
	GetValue_Lagrange(vector<T>& xSeq , vector<T>& ySeq, T x) 
	{
		int order = xSeq.size()-1;
		T y = 0;
		
		for (int i = 0; i < xSeq.size(); i++)
		{
			double l = 1;
			for (int j = 0; j <= order; j++)
			{
				if (j != i)
				{
					l = l*(x - xSeq.at(j))*1.0f / (xSeq.at(i) - xSeq.at(j));
				}
			}
			y += l * ySeq[i];
		}
		return y;
	}


};


