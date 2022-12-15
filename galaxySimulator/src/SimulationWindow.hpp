#pragma once

#include <cstdint>
#include <vector>
#include <SDL_ttf.h>

#include "SDLWindow.hpp"
#include "Space.hpp"

#include "RenderEllipse.hpp"
#include "RenderStars.hpp"

class SimulationWindow final : public SDLWindow
{
public:

	SimulationWindow();
	~SimulationWindow();

	virtual void StartSimulation() override;
	virtual void UpdateSimulation() override;

	void InitGL() override;
	int testOpenGL() override;
	void InitSimulation() override;

	void initSpaceVariables();
	void UpdateEllipses();
	void UpdateStars();

	void DrawStar();
	void DrawEllipse();
	

private:
	const float TimeStepSize = 100000.0f;
	Space space;
	RenderEllipse ellipsesRender;
	RenderStars starRender;

	float time;
};

