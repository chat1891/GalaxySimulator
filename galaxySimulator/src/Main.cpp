#include<iostream>

#include "SimulationWindow.hpp"
//#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{
	try
	{
		SimulationWindow wndMain;
		wndMain.Init(1500, 1000, 35000.0, "Galaxy");
		wndMain.MainLoop();
	}
	catch(std::exception& e)
	{
		std::cout << "Error in main func: " << e.what() << std::endl;
	}
	return 0;
}