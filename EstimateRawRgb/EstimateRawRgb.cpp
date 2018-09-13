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

template<typename T> void PolynomialEstimation(vector<T>& srgbInput, vector<T>& rawInput, vector<T>& srgbTest, vector<T>& rawOutcomes,bool isPrintData, vector<T>& rawTest)
{
	InversePolynomial* polynomial = new InversePolynomial;
	polynomial->TrainingProcess<double>(rawInput, srgbInput, 3);
	rawOutcomes = polynomial->GetPolyFitVal(srgbTest);
	cout << polynomial->GetFittingEquation() << endl;
	if(isPrintData) Disp::PrintOnScreen(srgbTest, rawTest, rawOutcomes);
	EvaluationMetric* eval = &EvaluationMetric();
	eval->CompareTwoDataSet<double, double>(srgbTest, rawTest, rawOutcomes);
	Disp::PrintGoodnessOfFitting(*eval, "Polynomial");

}
template<typename T> void LutEstimation(vector<T>& srgbInput, vector<T>& rawInput, vector<T>& srgbTest, vector<T>& rawOutcomes, bool isPrintData, vector<T>& rawTest)
{
	Lut1DInterpolation *lut = new Lut1DInterpolation(srgbInput, rawInput);
	rawOutcomes = lut->GetValue(srgbTest);
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
	IOHelper::ReadCameraResponseFileTovector(trainingData, rawR, rawG, rawB, srgbR, srgbG, srgbB, ReadingOption::Original);
	IOHelper::ReadCameraResponseFileTovector(testingData, testRawR, testRawG, testRawB, testSrgbR, testSrgbG, testSrgB, ReadingOption::Original);
	std::cout << "Loading camera reponse" << endl;
	if (isPrintInputData) Disp::PrintOnScreen(rawR, rawG, rawB, "Raw RGB");
	cout << "Polynomial fitting" << endl;
	PolynomialEstimation(srgbR, rawR, testSrgbR, estRawR,isPrintEstData,testRawR);
	cout << "1-D LUT interpolation" << endl;
	LutEstimation(srgbR, rawR, testSrgbR, estRawR,isPrintEstData, testRawR);

	getchar();
    return 0;
}

