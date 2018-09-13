#include "stdafx.h"
#include "LookUpTable.h"
#include <vector>
#include <map>
Lut1DInterpolation::Lut1DInterpolation()
{
}


Lut1DInterpolation::~Lut1DInterpolation()
{
}

void Lut1DInterpolation::ExtendDataTable()
{
	{
		int srgbMax = 256;
		std::vector<int> srgbRange(srgbMax);
		std::generate_n(srgbRange.begin(), srgbMax, [] { static int i{ 0 }; return i++; });
		FullSrgbDataTable = DataTable;
		std::map<double, double>::reverse_iterator it_map = DataTable.rbegin();
		while (it_map != DataTable.rend())
		{
			srgbRange.erase(srgbRange.begin() + (int)(it_map->first));
			it_map++;

		}
		for (vector<int>::iterator it = srgbRange.begin(); it != srgbRange.end(); it++)
		{
			double interpValue = GetValue(*it);
			FullSrgbDataTable.insert(make_pair((double)(*it), interpValue));
		}
	}
}

