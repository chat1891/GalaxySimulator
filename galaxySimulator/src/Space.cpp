#include "Space.hpp"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "BasicCalculation.hpp"
#include "Helper.hpp"
#include "Calculation.hpp"
#include "Star.hpp"

Space::Space(float radius,
	float radiusCore,
	float deltaAngle,
	float ex1,
	float ex2,
	int numStars)
	: excentricity_core(ex1)
	, excentricity_disk(ex2)
	, angleChange(deltaAngle)
	, centerRad(radiusCore)
	, rad(radius)
	, boundaryRad(radius * 2)
	, numStars(numStars)
	, perturbationsNum(0)
	, p_amp(0)
	, tempratureLow(4000)
	, _stars()
	, dustParticleSize(70)
{

}

Space::~Space()
{
}

void Space::InitStarsAndDust()
{
	_stars = std::vector<Star>();
	_stars.reserve(numStars);

	////1. init stars
	CumulativeDistributionFunction cdf;
	cdf.Init(
		1.0,				
		0.02,				
		rad / 3.0f,	
		centerRad,			
		0,					
		boundaryRad,		
		1000);		


	for (int i = 0; i < numStars; i++)
	{
		float rad = (float)cdf.valueFromProbability();
		auto star = Star();
		calculateStarProperties(star, rad, starType::star); //type 0 is star
		_stars.push_back(star);

	}
}

float Space::GetExcentricity(float r)
{
	if (r < centerRad)
	{
		return 1 + (r / centerRad) * (excentricity_core - 1);
	}
	else if (r > centerRad && r <= rad)
	{
		return excentricity_core + (r - centerRad) / (rad - centerRad) * (excentricity_disk - excentricity_core);
	}
	else if (r > rad && r < boundaryRad)
	{
		return excentricity_disk + (r - rad) / (boundaryRad - rad) * (1 - excentricity_disk);
	}
	else
		return 1;
}

float Space::GetFieldRad()
{
	return boundaryRad;
}

float Space::GetDustRenderSize()
{
	return dustParticleSize;
}

void Space::calculateStarProperties(Star& curStar, const float& radius, const int& type)
{
	curStar.a = radius;
	curStar.excentricity = GetExcentricity(radius);
	curStar.b = radius * curStar.excentricity;
	curStar.tiltAngle = GetAngularOffset(radius);
	curStar.theta0 = 360.0f * BasicCalculation::randomNum();
	curStar.velTheta = GetAngularVelocity(radius);

	curStar.type = type;
}

void Space::SetRadius(float radius)
{
	rad = radius;
}

void Space::SetRadBoundry(float radius)
{
	boundaryRad = radius;
}


void Space::SetCenterRad(float radius)
{
	centerRad = radius;
}

void Space::SetExcentricity_core(float excentricity)
{
	excentricity_core = excentricity;
}

void Space::SetExcentricity_disk(float excentricity)
{
	excentricity_disk = excentricity;
}

void Space::SetNumStars(int num)
{
	numStars = num;
}



void Space::SetAnguleChange(float rad)
{
	angleChange = rad;
}

void Space::SetDustParticleSize(int sz)
{
	dustParticleSize = sz;
}

void Space::SetPerturbationsNum(int n)
{
	perturbationsNum = n;
}

void Space::SetAmplitudeDamping(int a)
{
	p_amp = a;
}

void Space::SetTemperaturLow(int temp)
{
	tempratureLow = temp;
}

float Space::GetRad()
{
	return rad;
}

float Space::GetCoreRad()
{
	return centerRad;
}

float Space::GetAngularOffset(float rad)
{
	return rad * angleChange;
}

int Space::GetPerturbationsNum()
{
	return perturbationsNum;
}

float Space::GetPerturbationAmplitude()
{
	return p_amp;
}

const std::vector<Star>& Space::GetStars()
{
	return _stars;
}

float Space::GetAngularVelocity(float rad)
{
	float velocityKm_s = 0;  

	velocityKm_s = BasicCalculation::CalculateVelocity(rad);

	float deg_per_year = 2.0f * BasicCalculation::pi * rad * BasicCalculation::parsecsToKm;  
	float time = deg_per_year / (velocityKm_s * BasicCalculation::secondPerYear);	

	return 360.0f / time;                        
}

