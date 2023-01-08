#pragma once
#pragma once

#include <vector>


class CumulativeDistributionFunction final
{
public:
	CumulativeDistributionFunction();
	double valueFromProbability();

	void Init(double I0, double k, double a, double RBulge, double min, double max, int nSteps);

private:
	double _MinR;
	double _MaxR;
	int _numSteps;

	double _I0;
	double _k;
	double _a;
	double _BulgeR;

	std::vector<double> _y_1;
	std::vector<double> _x_1;

	void CreateCDF();
	void InitVectors();

	double InnerIntensity(double R, double I0, double k);
	double OuterIntensity(double R, double I0, double a);
	double Intensity(double x);

	void CalculateX1_Y1();
	void nomalizeVector();
};
