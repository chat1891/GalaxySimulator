#pragma once
#include <cstdint>

/// <summary>
/// theta0: angular position, initial position on ellipse
/// velThera: angular velocity
/// Type 0:star
/// </summary>
class Star
{
public:
	float theta0; 
	float velTheta;
	float tiltAngle;  
	float a;          
	float b;          
	int32_t type;	 
	float excentricity; 

}; 
