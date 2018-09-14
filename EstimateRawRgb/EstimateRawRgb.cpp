// EstimateRawRgb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "IOhelper.h";
#include "InversePolynomial.h"
#include "LookUpTable.h"
#include "InfoDisplay.h"
using namespace std;
enum EstimationMethod
{
	Polynomial,
	LUT_1D,
};
template<typename T> void PolynomialEstimation(vector<T>& srgbInput, vector<T>& rawInput, vector<T>& srgbTest, vector<T>& rawOutcomes,bool isPrintData, vector<T>& rawTest)
{
	InversePolynomial* polynomial = new InversePolynomial;
	bool isSrgbLogarithmic = false;
	bool isRawRgbLogarithmic = true;
	if (isRawRgbLogarithmic)
	{
		std::transform(rawInput.begin(), rawInput.end(), rawInput.begin(), [](T x)->double {return log(x); });
	}
	if (isSrgbLogarithmic)
	{
		std::transform(srgbInput.begin(), srgbInput.end(), srgbInput.begin(), [](T x)->double {return log(x); });
		std::transform(srgbTest.begin(), srgbTest.end(), srgbTest.begin(), [](T x)->double {return log(x); });
	}
	int polynomialOrder = 3;
	polynomial->TrainingProcess<double>(rawInput, srgbInput, polynomialOrder);
	rawOutcomes = polynomial->GetPolyFitVal(srgbTest);
	if (isRawRgbLogarithmic)
	{
		std::transform(rawOutcomes.begin(), rawOutcomes.end(), rawOutcomes.begin(), [](T x)->double {return exp(x); });
	}
	if (isSrgbLogarithmic)
	{
		std::transform(srgbTest.begin(), srgbTest.end(), srgbTest.begin(), [](T x)->double {return exp(x); });
	}
	cout << polynomial->GetFittingEquation() << endl;
	if(isPrintData) Disp::PrintOnScreen(srgbTest, rawTest, rawOutcomes);
	EvaluationMetric* eval = &EvaluationMetric();
	eval->CompareTwoDataSet<double, double>(srgbTest, rawTest, rawOutcomes);
	Disp::PrintGoodnessOfFitting(*eval, "Polynomial");

}
template<typename T> void LutEstimation(vector<T>& srgbInput, vector<T>& rawInput, vector<T>& srgbTest, vector<T>& rawOutcomes, bool isPrintData, vector<T>& rawTest)
{
	bool isSrgbLogarithmic = false;
	bool isRawRgbLogarithmic = true;
	if (isRawRgbLogarithmic)
	{
		std::transform(rawInput.begin(), rawInput.end(), rawInput.begin(), [](T x)->double {return log(x); });
	}
	if (isSrgbLogarithmic)
	{
		std::transform(srgbInput.begin(), srgbInput.end(), srgbInput.begin(), [](T x)->double {return log(x); });
		std::transform(srgbTest.begin(), srgbTest.end(), srgbTest.begin(), [](T x)->double {return log(x); });
	}
	Lut1DInterpolation *lut = new Lut1DInterpolation(srgbInput, rawInput);
	rawOutcomes = lut->GetValue(srgbTest);
	if (isRawRgbLogarithmic)
	{
		std::transform(rawOutcomes.begin(), rawOutcomes.end(), rawOutcomes.begin(), [](T x)->double {return exp(x); });
	}
	if (isSrgbLogarithmic)
	{
		std::transform(srgbTest.begin(), srgbTest.end(), srgbTest.begin(), [](T x)->double {return exp(x); });
	}
	if (isPrintData) Disp::PrintOnScreen(srgbTest, rawTest, rawOutcomes);
	EvaluationMetric *eval = &EvaluationMetric();
	eval->CompareTwoDataSet<double, double>(srgbTest, rawTest, rawOutcomes);
	Disp::PrintGoodnessOfFitting(*eval, "LUT");

}
int main()
{
	std::string trainingData = "CCDT_Chart.txt";
	std::string testingData = "CCSG_Chart.txt";	
	bool isPrintInputData = false;
	bool isPrintEstData = false;
	vector<double> rawR, rawG, rawB;
	vector<double> srgbR, srgbG, srgbB;
	vector<double> testRawR, testRawG, testRawB;
	vector<double> testSrgbR, testSrgbG, testSrgB;
	vector<double> estRawR, estRawG, estRawB;
	std::cout << "Loading camera reponse" << endl;
	IOHelper::ReadCameraResponseFileTovector(trainingData, rawR, rawG, rawB, srgbR, srgbG, srgbB);
	IOHelper::ReadCameraResponseFileTovector(testingData, testRawR, testRawG, testRawB, testSrgbR, testSrgbG, testSrgB);
	if (isPrintInputData) Disp::PrintOnScreen(rawR, rawG, rawB, "Raw RGB");
	EstimationMethod method = EstimationMethod::Polynomial;
	switch (method)
	{
	case Polynomial:
		cout << "Polynomial fitting" << endl;
		PolynomialEstimation(srgbR, rawR, testSrgbR, estRawR, isPrintEstData, testRawR);
		break;
	case LUT_1D:
		cout << "1-D LUT interpolation" << endl;
		LutEstimation(srgbR, rawR, testSrgbR, estRawR, isPrintEstData, testRawR);
		break;
	default:
		cout << "Polynomial fitting" << endl;
		PolynomialEstimation(srgbR, rawR, testSrgbR, estRawR, isPrintEstData, testRawR);
		break;
		break;
	}



	getchar();
    return 0;
}

