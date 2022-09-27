#pragma once

#include <cstdint>
#include <vector>
#include <SDL_ttf.h>

#include "SDLWindow.hpp"

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
};

