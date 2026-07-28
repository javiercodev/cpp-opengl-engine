#pragma once
#include <memory>

// Forward declaration to avoid pulling in owindow.h (and Windows.h) here
class OWindow;

// Top-level owner of the game loop and the main window
class OGame
{
public:
	OGame();
	~OGame();

	void run();   // blocks until m_isRunning is false or the window closes
	void quit();  // signals run() to stop on the next loop iteration
protected:
	bool m_isRunning = true;
	std::unique_ptr<OWindow> m_display; // owns the window; auto-cleaned on destruction
};