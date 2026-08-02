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
		// Log the error message and exit gracefully.
		// e.what() is narrow (const char*); the wide-stream overload of
		// operator<< widens it automatically, so this still prints correctly.
		std::wclog << e.what() << std::endl;
		return -1;
	}
	return 0;
}