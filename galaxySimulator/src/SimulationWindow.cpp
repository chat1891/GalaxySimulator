#include "SimulationWindow.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cstddef>
#include <iostream>


SimulationWindow::SimulationWindow()
	: SDLWindow()
	, starRender(GL_FUNC_ADD, GL_ONE, GL_STATIC_DRAW)
	, space()
	, ellipsesRender(2) 
{

}

SimulationWindow::~SimulationWindow()
{

}

void SimulationWindow::StartSimulation()
{	
	AdjustCamera();
	GET_GLERROR;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GET_GLERROR;
	DrawStar();
	//DrawEllipse();
	
	SDL_GL_SwapWindow(_pSdlWnd);
}

void SimulationWindow::UpdateSimulation()
{
	time += SimulationWindow::TimeStepSize;
	UpdateEllipses();
	UpdateStars();
}

void SimulationWindow::InitGL()
{
	glViewport(0, 0, GetWidth(), GetHeight());

	ellipsesRender.Init();
	starRender.Init();

	glClearColor(0.0f, .0f, 0.08f, 0.0f);
	SetCameraOrientation({ 0, 1, 0 });
}

int SimulationWindow::testOpenGL()
{
	return 0;
}


void SimulationWindow::InitSimulation()
{
	initSpaceVariables();
	GET_GLERROR;
	space.InitStarsAndDust();
	GET_GLERROR;
}

void SimulationWindow::initSpaceVariables()
{
	space.SetAnguleChange(0.0004f);
	space.SetPerturbationsNum(2);
	space.SetAmplitudeDamping(40);

	space.SetCenterRad(4000);
	space.SetRadius(13000);

	space.SetExcentricity_core(0.85f);
	space.SetExcentricity_disk(0.95f);

	space.SetNumStars(100000);

	space.SetDustParticleSize(70);
	space.SetTemperaturLow(4000);
}


void SimulationWindow::UpdateStars()
{
	std::vector<StarVertex> vert;
	std::vector<int> idx;
	const auto& stars = space.GetStars();

	float a = 1;
	ColorVariables color = { 1,1,1,a };
	for (int i = 0; i < stars.size(); i++)
	{
		//can add color here to change across the galaxy
		color = { 0,1.2, 2, a };
		idx.push_back((int)vert.size());
		vert.push_back({ stars[i], color });
	}
	starRender.CreateBuffer(vert, idx, GL_POINTS);

}

void SimulationWindow::DrawStar()
{
	int featuresStar = 15;
	starRender.UpdateStarVariables(time, space.GetPerturbationsNum(), space.GetPerturbationAmplitude(), (int)space.GetDustRenderSize(), featuresStar);
	starRender.Draw(viewMatrix, projectionMatrix);
}

void SimulationWindow::DrawEllipse()
{
	ellipsesRender.Draw(viewMatrix, projectionMatrix);
}


void SimulationWindow::UpdateEllipses()
{
	std::vector<ColourVertex> vert;
	std::vector<int> idx;

	int num = 100;
	float dr = space.GetFieldRad() / num;
	for (int i = 0; i <= num; ++i)
	{
		float r = dr * (i + 1);
		float angleDegree = BasicCalculation::rad_degree * space.GetAngularOffset(r);
		float A = r;
		float B = r * space.GetExcentricity(r);
		ColorVariables c = { 1, 0, 0, 1 };
		ellipsesRender.calculateEllipseVertices(vert, idx, A,B, angleDegree,
			space.GetPerturbationsNum(), space.GetPerturbationAmplitude(), c);
	}

	ellipsesRender.CreateBuffer(vert, idx, GL_LINE_STRIP);
}
