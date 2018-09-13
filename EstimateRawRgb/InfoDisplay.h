
#include <iostream>
using namespace std;
namespace Disp
{
	static void PrintBasicInformation(char* trainingSampleName, char* testingSampleName, char* method)
	{
		cout << "Training Sample = \t" << trainingSampleName << endl;
		cout << "Testing Sample = \t" << testingSampleName << endl;
		cout << "Training Method = \t" << method << endl;

	}
	template<typename T> static void PrintOnScreen(vector<T> &r, vector<T> &g, vector<T> &b, char* colorspace)
	{
		cout << "Red\t" << "Green\t" << "Blue\t" << "Color space: " + string(colorspace) << endl;
		cout << "===============================" << endl;
		vector<T>::iterator it_r = r.begin();
		vector<T>::iterator it_g = g.begin();
		vector<T>::iterator it_b = b.begin();
		while (it_r != r.end())
		{
			cout << *(it_r++) << "\t" << *(it_g++) << "\t" << *(it_b++) << "\t" << endl;
		}
	}
	template<typename T> static void PrintOnScreen(vector<T> &x, vector<T> &y, vector<T> &est)
	{
		cout << "===============================" << endl;
		cout << "X = \t" << "Y = \t" << "Fit Outcomes = \t" << endl;
		cout << "===============================" << endl;
		vector<T>::iterator it_x = x.begin();
		vector<T>::iterator it_y = y.begin();
		vector<T>::iterator it_est = est.begin();
		while (it_x != x.end())
		{
			cout << *(it_x++) << "\t" << *(it_y++) << "\t" << *(it_est++) << "\t" << endl;
		}
	}
	static void PrintGoodnessOfFitting(const EvaluationMetric& eval, char* desp)
	{
		cout << "===============================" << endl;
		cout << "Data description = \t" << desp << endl;
		cout << "Number of testing data = \t" << eval.DataCount << endl;
		cout << "[RMSE] = \t" << eval.RMSE << endl;
		cout << "[SSE] = \t" << eval.SSE << endl;
		cout << "[R-square] = \t" << eval.RSquare << endl;
		cout << "[Max descripancy]:" << endl;
		cout << "X = \t" << std::get<0>(eval.MaximumAbsoluteDescripancy) << "\tMeasured value = " << std::get<1>(eval.MaximumAbsoluteDescripancy) << "\tFitted value = " << std::get<2>(eval.MaximumAbsoluteDescripancy) << endl;
		cout << "===============================" << endl;
	}
}
