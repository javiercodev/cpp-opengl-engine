#include <ogl3d/game/ogame.h>
#include <ogl3d/window/owindow.h>
#include <ogl3d/graphics/ographicsengine.h>
#include <Windows.h>

// Win32-specific half of OGame: the message pump and the per-frame render
// calls. Split from ogame.cpp so the platform-agnostic lifecycle code
// doesn't need to include Windows.h.

void OGame::run()
{
	onCreate();

	// Main loop: keeps going until quit() is called or the window is closed
	while (m_isRunning)
	{
		MSG msg = {};
		// Pulls at most one pending message per loop iteration (non-blocking);
		// if the queue is empty, falls straight through to rendering
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg); // routes the message to WndProc
		}

		// Render one frame: clear, let the game draw, then present it
		m_graphicsEngine->clear(Ovec4(0.2f, 0.3f, 0.3f, 1.0f));
		onUpdate();
		m_display->present(false);

		// TODO: replace with real frame timing once a clock/delta-time exists
		Sleep(1);
	}

	onQuit();
}