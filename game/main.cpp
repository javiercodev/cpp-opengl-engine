#include <ogl3d/game/ogame.h>
#include <iostream>

int main()
{
	// Entry point: owns nothing else, just bootstraps the engine
	try
	{
		OGame game;
		game.run();
	}
	catch (const std::exception& e)
	{
		// Log the error message and exit gracefully
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}