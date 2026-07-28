#include <ogl3d/game/ogame.h>

int main()
{
	// Entry point: owns nothing else, just bootstraps the engine
	OGame game;
	game.run();
	return 0;
}