#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>
#include <Windows.h>

// Win32-specific half of OGame: the message pump. Split from ogame.cpp so
// the platform-agnostic lifecycle code doesn't need to include Windows.h.

void OGame::run()
{
	onCreate();
	// Main loop: keeps going until quit() is called or the window is closed
	while (m_isRunning)
	{
		MSG msg = {};
		// Pulls at most one pending message per loop iteration (non-blocking);
		// if the queue is empty, falls straight through to onUpdate()
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg); // routes the message to WndProc
			}
		}

		// TODO: replace with real frame timing once rendering is in place
		onUpdate();
	}
	onQuit();
}