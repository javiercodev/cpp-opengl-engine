#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>

// Platform-agnostic lifecycle. The actual Win32 message loop lives in
// cwin32game.cpp, keeping this file free of Windows.h.

OGame::OGame()
{
	// Graphics engine must exist before the window: it loads the WGL
	// extensions (via a dummy context) that OWindow's constructor relies on.
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();

	m_display->makeCurrentContext();
}

OGame::~OGame()
{
	// std::unique_ptr automatically manages the memory
}

void OGame::onCreate()
{
	m_graphicsEngine->clear(Ovec4(1, 0, 0, 1));

	m_display->present(false); // present the red screen without vsync
}

void OGame::onUpdate()
{
}

void OGame::onQuit()
{
}

void OGame::quit()
{
	m_isRunning = false;
}