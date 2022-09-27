#include<iostream>

#include "SimulationWindow.hpp"
//#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{
	try
	{
		SimulationWindow wndMain;
	}
	catch(std::exception& exc)
	{
		std::cout << "Fatal error: " << exc.what() << std::endl;
	}
	return 0;
}