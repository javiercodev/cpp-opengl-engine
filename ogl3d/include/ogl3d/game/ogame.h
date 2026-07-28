#pragma once
#include <memory>

// Forward declaration to avoid pulling in owindow.h (and Windows.h) here
class OWindow;
// Forward declaration: OGame only needs a pointer/reference here, no need
// to pull in ographicsengine.h
class OGraphicsEngine;

// Top-level owner of the game loop, the window, and the graphics engine
class OGame
{
public:
	OGame();
	~OGame();

	// Overridable hooks: derived games plug their own logic in without touching run()
	virtual void onCreate(); // called once before the loop starts
	virtual void onUpdate(); // called once per loop iteration
	virtual void onQuit();   // called once after the loop ends

	void run();   // blocks until m_isRunning is false or the window closes
	void quit();  // signals run() to stop on the next loop iteration
protected:
	bool m_isRunning = true;
	std::unique_ptr<OGraphicsEngine> m_graphicsEngine; // owns the graphics engine; auto-cleaned on destruction
	std::unique_ptr<OWindow> m_display; // owns the window; auto-cleaned on destruction
};