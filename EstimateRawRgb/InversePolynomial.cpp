#include "stdafx.h"
#include "InversePolynomial.h"


InversePolynomial::InversePolynomial()
{
}


InversePolynomial::~InversePolynomial()
{
}

string InversePolynomial::GetFittingEquation()
{
	string s = std::to_string(fitCoeff[0]);
	for (int i = 1; i <= Order; i++)
	{
		s += "+" + to_string(fitCoeff[i]) + " x^" + to_string(i);
	}
	return s;
}



