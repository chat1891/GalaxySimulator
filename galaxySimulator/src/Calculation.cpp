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
	nomalizeVector();

	CalculateX2_Y2();
}

void CumulativeDistributionFunction::InitVectors()
{
	_x_1.clear();
	_y_1.clear();
	_x_2.clear();
	_y_2.clear();
	_m_1.clear();
	_m_2.clear();

	_y_1.push_back(0.0);
	_x_1.push_back(0.0);

	_x_2.push_back(0.0);
	_y_2.push_back(0.0);
}


double CumulativeDistributionFunction::valueFromProbability()
{
	int sz = _m_2.size();
	double frac = 1.0 / (sz);

	int i = rand() % sz;
	double factor = i * frac;

	assert(i >= 0 && i < (int)_m_2.size());
	return(_y_2[i] + _m_2[i]*factor);
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
	if (x < _BulgeR)
	{
		return InnerIntensity(x, _I0, _k);
	}
	return OuterIntensity(x - _BulgeR, InnerIntensity(_BulgeR, _I0, _k), _a);
}

void CumulativeDistributionFunction::CalculateX1_Y1()
{
	double gap = (_MaxR - _MinR) / _numSteps;
	double x_r = 0;
	double y_r = 0;

	for (int i = 0; i < _numSteps; i += 2)
	{
		//Simpson rule
		x_r = gap * (i + 2); //r
		double simpson1 = Intensity(_MinR + i * gap);
		double simpson2 = 4 * Intensity(_MinR + (i + 1) * gap);
		double simpson3 = Intensity(_MinR + (i + 2) * gap);
		y_r += gap / 3 * (simpson1 + simpson2 + simpson3);

		_m_1.push_back((y_r - _y_1.back()) / (2 * gap));
		_x_1.push_back(x_r);
		_y_1.push_back(y_r);
	}
	_m_1.push_back(0.0);
}

void CumulativeDistributionFunction::CalculateX2_Y2()
{
	double p = 0;
	double gap = 1.0 / _numSteps;
	double y_r = 0;
	for (int i = 1, j = 0; i < _numSteps; ++i)
	{
		p = (double)i * gap;

		for (; _y_1[j + 1] <= p; j++) {}

		y_r = _x_1[j] + (p - _y_1[j]) / _m_1[j];

		_m_2.push_back((y_r - _y_2.back()) / gap);
		_x_2.push_back(p);
		_y_2.push_back(y_r);
	}
	_m_2.push_back(0.0);
}

void CumulativeDistributionFunction::nomalizeVector()
{
	for (std::size_t i = 0; i < _y_1.size(); ++i)
	{
		_y_1[i] /= _y_1.back();
		_m_1[i] /= _y_1.back();
	}
}
