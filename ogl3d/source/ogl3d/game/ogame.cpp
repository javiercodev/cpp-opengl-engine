#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <Windows.h>

OGame::OGame()
{
	m_display = std::unique_ptr<OWindow>(new OWindow());
}

OGame::~OGame()
{
	// std::unique_ptr automatically manages the memory
}

void OGame::run()
{
	MSG msg;
	// Main loop: keeps going until quit() is called or the window is closed
	while (m_isRunning && !m_display->isClosed())
	{
		// Drain all pending Windows messages without blocking (PM_REMOVE)
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // routes the message to WndProc
		}

		// TODO: replace with real frame timing once rendering is in place
		Sleep(1);
	}
}

void OGame::quit()
{
	m_isRunning = false;
}