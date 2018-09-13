#pragma once
#include <vector>;
using namespace std;
#include <typeinfo>
#include <math.h>  
enum SolverType
{
	GaussianElimination = 0,
	GaussSiedel,
	
};
class LinearEquationSolver
{
public:
	LinearEquationSolver();
	~LinearEquationSolver();
	template<typename T>static const vector<double> Solve(int n, double* A, T* b, SolverType solver = GaussianElimination)
	{
		vector<double> X(n);
		switch (solver)
		{
		case GaussianElimination:
			GaussianSolver<double,T>(n, A, b, X);
			break;
		case GaussSiedel:
				break;
		default:
			break;
		}
		return X;
	}
private:
	template<typename Ta, typename Tb> static void GaussianSolver(int dimension, Ta* A, Tb* b, vector<double>& x)
	{
		int i, j, row, r;
		double maxColumnElement;

		for (row = 0; row < dimension - 1; row++)
		{
			maxColumnElement = fabs(A[row*dimension + row]); /*fidimensiond maxElementmum*/
			r = row;
			for (i = row + 1; i < dimension - 1; i++) {
				if (maxColumnElement < fabs(A[i*dimension + i]))
				{
					maxColumnElement = fabs(A[i*dimension + i]);
					r = i;
				}
			}
			if (r != row) {
				for (i = 0; i < dimension; i++)         /*chadimensionge array:A[row]&A[r] */
				{
					maxColumnElement = A[row*dimension + i];
					A[row*dimension + i] = A[r*dimension + i];
					A[r*dimension + i] = maxColumnElement;
				}
			}
			maxColumnElement = b[row];                    /*chadimensionge array:b[row]&b[r]     */
			b[row] = b[r];
			b[r] = maxColumnElement;
			for (i = row + 1; i < dimension; i++)
			{
				for (j = row + 1; j < dimension; j++)
					A[i*dimension + j] -= A[i*dimension + row] * A[row*dimension + j] / A[row*dimension + row];
				b[i] -= A[i*dimension + row] * b[row] / A[row*dimension + row];
			}
		}

		for (i = dimension - 1; i >= 0; x[i] /= A[i*dimension + i], i--)
			for (j = i + 1, x[i] = b[i]; j < dimension; j++)
				x[i] -= A[i*dimension + j] * x[j];
	}
public:
	template<typename Ta, typename Tb> static vector<double> GaussianSolver(vector<Ta>& A, vector<Tb>& b)
	{
		std::vector<Ta>::iterator it;
		Ta maxColumnElement;
		int maxElementIndex;
		int currentDiagIndex;
		int dimension = b.size();
		vector<double> X(dimension);
		for (int row = 0; row < dimension-1; row++)
		{
			currentDiagIndex = maxElementIndex = row*(dimension+1);//Search the maximum element in the column
			maxColumnElement = fabs(A[currentDiagIndex]);
			for (int i = currentDiagIndex + dimension; i< A.size(); i += dimension)
			{
				if (fabs(A[i]) > maxColumnElement)
				{
					maxColumnElement = fabs(A[i]);
					maxElementIndex = i;
				}
			}
			if (maxElementIndex != currentDiagIndex) // Switch the rows if the maximum is not the first
			{
				for (int i = 0; i < dimension; i++)
				{
					std::iter_swap(A.begin() + currentDiagIndex + i, A.begin() + maxElementIndex + i);
				}
				std::iter_swap(b.begin() + row, b.begin() + maxElementIndex / dimension);
				
			}
			maxElementIndex = currentDiagIndex;
			for (int i = row + 1; i < dimension; ++i) //Elimnating elements 
			{
				double elmtCoeff = -A[i*dimension + row] / A[maxElementIndex];
				for ( it = A.begin() + i*dimension+row; it != A.begin() + (i+1) * dimension; ++it)
				{
					*it += elmtCoeff * *(it - (i-row) * dimension);
				}
				b[i] += elmtCoeff * b[row];
			}

		}
		X[dimension - 1] = b[dimension - 1] / A[dimension*dimension - 1];
		for (int row = dimension - 2; row >= 0; row--)
		{
			double sumTemp = 0;
			for (int column = row + 1; column < dimension; column++)
			{
				sumTemp += A[row*dimension + column] * X[column];
			}
			X[row] = (b[row] - sumTemp) / A[row*dimension + row];
				
		}
		return X;
	}
};


