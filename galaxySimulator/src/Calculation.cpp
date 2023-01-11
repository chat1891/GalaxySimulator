#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <stdexcept>

#include "Calculation.hpp"


CumulativeDistributionFunction::CumulativeDistributionFunction()
{
}

/// <summary>
/// 
/// </summary>
/// <param name="I0"></param> max intensity
/// <param name="k"></param> bulge
/// <param name="a"></param> scale length of disc
/// <param name="RBulge"></param> radius of bulge
/// <param name="min"></param> min of intensity
/// <param name="max"></param> max of intensity  
/// <param name="nSteps"></param> number of steps
void CumulativeDistributionFunction::Init(double I_0, double k, double a, double Radius_Bulge, double min, double max, int nSteps)
{
	_I0 = I_0;
	_k = k;
	_a = a;
	_BulgeR = Radius_Bulge;

	_MinR = min;
	_MaxR = max;
	_numSteps = nSteps;

	CreateCDF();
}

void CumulativeDistributionFunction::CreateCDF()
{
	InitVectors();

	CalculateX1_Y1();
	//nomalizeVector();
}

void CumulativeDistributionFunction::InitVectors()
{
	_x_1.clear();
	_y_1.clear();

	_y_1.push_back(0.0);
	_x_1.push_back(0.0);
}

double CumulativeDistributionFunction::InnerIntensity(double R, double I0, double r_e)
{
	return I0 * exp(-r_e * pow(R, 0.25));
}


double CumulativeDistributionFunction::OuterIntensity(double R, double I0, double a)
{
	return I0 * exp(-R / a);
}


double CumulativeDistributionFunction::Intensity(double x)
{

	return InnerIntensity(x, _I0, _k);

}

void CumulativeDistributionFunction::nomalizeVector()
{
	for (std::size_t i = 0; i < _y_1.size(); ++i)
	{
		_y_1[i] /= _y_1.back();
	}
}

void CumulativeDistributionFunction::CalculateX1_Y1()
{
	double gap = _MaxR / _numSteps;
	double x_r = 0;
	double y_r = 0;

	for (int i = 0; i < _numSteps; i += 1)
	{
		//Simpson rule
		x_r = gap * (i); //r
		double simpson1 = Intensity(i * gap);
		double simpson2 = 4 * Intensity((i + 1) * gap);
		double simpson3 = 2 * Intensity((i + 2) * gap);
		y_r += gap / 3 * (simpson1 + simpson2 + simpson3);


		_x_1.push_back(x_r);
		_y_1.push_back(y_r);
	}

}

double CumulativeDistributionFunction::valueFromProbability()
{
	int sz = _y_1.size();
	int i = rand() % sz;
	double factor = 1 / sz;

	assert(i >= 0 && i < sz);

	return _y_1[i];
	//return(_y_1[i] *_MaxR);
}
