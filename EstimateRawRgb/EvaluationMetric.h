#pragma once
using namespace std;
#include <tuple>;
#include <vector>;
class EvaluationMetric
{
public:
	EvaluationMetric();
	~EvaluationMetric();
public:
	double StandardDeviation;
	double RMSE;
	double SSE;
	double SSR;
	double RSquare;
	int DataCount;
	tuple<double, double, double> MaximumAbsoluteDescripancy;
	tuple<double, double, double> MinimumAbsoluteDescripancy;
	
public:
	template<typename Ta, typename Tb> void CompareTwoDataSet(const vector<Ta> &x, const vector<Tb>& seqOrginal, const vector<Tb> seqEstimated)
	{
		DataCount = x.size();
		SSE = 0;
		SSR = 0;
		RMSE = 0;
		int dataLength = seqOrginal.size();
		auto it_A = seqOrginal.begin();
		auto it_B = seqEstimated.begin();
		auto it_X = x.begin();
		double max = 0;
		double min = 65535;
		double descripancy;
		double seqAMean = Mean(seqOrginal);
		double seqBMean = Mean(seqEstimated);
		while (it_A != seqOrginal.end() && it_B != seqEstimated.end())
		{
			descripancy = fabs(*(it_A) - *(it_B));
			SSE += pow(descripancy, 2);
			SSR += pow((*it_B - seqAMean), 2);
			if (descripancy > max)
			{
				max = descripancy;
				MaximumAbsoluteDescripancy = make_tuple(*it_X, *it_A, *it_B);
			}
			if (descripancy < min)
			{
				min = descripancy;
				MinimumAbsoluteDescripancy = make_tuple(*it_X, *it_A, *it_B);
			}
			it_X++;
			it_A++;
			it_B++;
		}
		RSquare = 1 - (SSE / (SSE + SSR));
		RMSE = sqrt(SSE / dataLength);
	}
private:
	template<typename T> static double Mean(const vector<T>& seq)
	{
		double sum = 0;
		for (auto const& value: seq)
		{
			sum += value;
		}
		return sum / seq.size();
	}
};

