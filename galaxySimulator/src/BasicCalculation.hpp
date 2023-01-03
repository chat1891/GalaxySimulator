#pragma once

#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include <GL/glew.h>

#include "Helper.hpp"
#include <SDL_stdinc.h>

class BasicCalculation
{
public:

	static const float parsecsToKm;

	static const float secondPerYear;

	static const float degree_rad;
	static const float rad_degree;
	static const float gravity;

	static const float pi;

	static inline unsigned int PowerTwoFloor(unsigned int val)
	{
		unsigned int power = 2, nextVal = power * 2;

		while ((nextVal *= 2) <= val)
		{
			power = power *2;
		}

		return power * 2; 
	}

	static inline float randomNum()
	{
		return (float)std::rand() / RAND_MAX;
	}


	static float CalculateVelocity(float r)
	{
		float MZ = 100;
		return 20000.0f * (float)std::sqrt(BasicCalculation::gravity * (RhoHalos(r) + MassOfDisc(r) + MZ) / r);
	}


private:
	static double MassOfDisc(double r)
	{
		float dia = 2000;		
		float rhoSo = 1;	
		float radH = 2000;	
		return (float)rhoSo * (float)std::exp(-r / radH) * (r * r) * BasicCalculation::pi * dia;
	}

	static float RhoHalos(const float& r)
	{
		float rhoH_0 = 0.15f;	
		float radS = 2500;		
		
		return (float)rhoH_0 / (float)(1 + std::pow(r / radS, 2)) * Four_Pie_G_R(r);
	}

	static float Four_Pie_G_R(const float& r)
	{
		return (float)(4 * BasicCalculation::pi * std::pow(r, 3) / 3);
	}
};