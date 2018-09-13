#pragma once
using namespace std;
#define MaximumItemNumber 20
#include <vector>
#include "LinearEquationSolver.h"
#include "math.h"
#include "EvaluationMetric.h"
#include <string>
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
	os << "[";
	for (int i = 0; i < v.size(); ++i) {
		os << v[i];
		if (i != v.size() - 1)
			os << ", ";
	}
	os << "]\n";
	return os;
}
class InversePolynomial
{
public:
	InversePolynomial();
	InversePolynomial(int order)
	{
		Order = order;
		fitCoeff = vector<double>(order + 1);
	}
	~InversePolynomial();
public:
	template<typename T> void GenerateMatrix(const T* srgb, const T* rawRgb, const size_t sampleNumber, const int order, vector<double>& A, vector<double>& b) 
	{
		unsigned sqMarDimension = order + 1;
		vector<double> xPower(sqMarDimension * 2-1);
		vector<double> xMatrix(sqMarDimension*sqMarDimension);	
		vector<double> yMatrix(sqMarDimension);

		for (unsigned j = 0; j < sampleNumber; ++j)
		{
			for (unsigned i = 0; i <= order; ++i)
			{
				double xp = pow(srgb[j], i);
				yMatrix[i] += rawRgb[j] * xp;
				xPower[i] += xp;
			}
			for (unsigned i = order + 1; i <= order * 2;i++)
			{
				xPower[i] += pow(srgb[j], i);
			}
		}
		for (unsigned i = 0; i <= order; i++)
		{
			for (unsigned j = 0; j <= order; j++)
			{
				xMatrix[i*sqMarDimension + j] = xPower[i + j];
			}
		}
		yMatrix.resize(sqMarDimension);
		A = xMatrix;
		b = yMatrix;
	}

	template <typename T> void TrainingProcess(vector<T>& inputRawRgb,vector<T>& inputSrgb, int order)
	{
		int sampleNumber = inputSrgb.size();
		this->Order = order;
		vector<double> A;
		vector<double> b;
		GenerateMatrix<T>(&inputSrgb[0], &inputRawRgb[0], sampleNumber, order, A, b);// Least Squared Method
		fitCoeff = LinearEquationSolver::GaussianSolver(A, b);
		vector<double> estimated = GetPolyFitVal(inputSrgb);
		SelfEvaluation.CompareTwoDataSet(inputSrgb, inputRawRgb, estimated);
		//cout << "X = " << fitCoeff;
	}
	template <typename T> vector<double> GetPolyFitVal(const vector<T>& inputSrgb)
	{
		size_t dataLength = inputSrgb.size();
		vector<double> polyfitValues;
		for (auto const& value : inputSrgb)
		{
			polyfitValues.push_back(GetPolyFitVal(value));
		}
		return polyfitValues;
	}
	template <typename T> double GetPolyFitVal(T x)
	{
		double v = 0;
		for (unsigned i = 0; i <= Order; i++)
		{
			v += fitCoeff[i] * pow(x, i);
		}
		return v;
	}
public:
	std::vector<double> fitCoeff;
	int Order;
	EvaluationMetric SelfEvaluation;
	string GetFittingEquation();
};

