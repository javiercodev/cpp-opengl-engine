#pragma once
#include <memory>
#include <ogl3d/oprerequisites.h>
#include <chrono> 

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

	// Demo resources: Renamed to polygonVAO for general geometry; 
	// Added Uniform Buffer support for modern shader data management.
	OVertexArrayObjectPtr m_polygonVAO;
	OUniformBufferPtr m_uniform;
	OShaderProgramPtr m_shader;

	// Timing and Animation:
	// m_previousTime tracks the timestamp of the last frame for delta time calculation.
	// m_scale is a demo parameter used to animate the polygon (e.g., pulsing or scaling logic).
	std::chrono::system_clock::time_point m_previousTime;
	f32 m_scale = 1.0f; // legacy: kept for compatibility with older code

	// Time accumulator used for procedural animation (separate from m_scale)
	f32 m_time = 0.0f;
};