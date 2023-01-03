#pragma once

#include<vector>
#include "Helper.hpp"
#include "Star.hpp"

class Space
{
public:
	enum starType
	{
		star = 0,
		dust
	};

	Space(
		float radius = 17000,
		float radiusCore = 6000,
		float deltaAngle = 0.019,
		float ex1 = 0.8,
		float ex2 = 1,
		int numStars = 60000);
	~Space();
	const std::vector<Star>& GetStars();

	void InitStarsAndDust();

	float GetFieldRad();
	float GetRad();
	float GetCoreRad();
	float GetExcentricity(float rad);
	float GetAngularVelocity(float rad);
	float GetAngularOffset(float rad);
	float GetDustRenderSize();
	int GetPerturbationsNum();
	float GetPerturbationAmplitude();
	void calculateStarProperties(Star& curStar, const float& radius, const int& type);

	void SetRadBoundry(float radius);
	void SetRadius(float radius);
	void SetCenterRad(float radius);
	void SetExcentricity_core(float excentricity);
	void SetExcentricity_disk(float excentricity);
	void SetNumStars(int num);
	void SetAnguleChange(float rad);
	void SetDustParticleSize(int sz);
	void SetPerturbationsNum(int n);
	void SetAmplitudeDamping(int a);
	void SetTemperaturLow(int temp);

private:
	float excentricity_core;
	float excentricity_disk;         

	float angleChange;   

	float centerRad;       
	float rad;      
	float boundaryRad;    
	float dustParticleSize;

	int numStars;          

	int perturbationsNum;
	float p_amp;

	float tempratureLow;

	std::vector<Star> _stars;  

};