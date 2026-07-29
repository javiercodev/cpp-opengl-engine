#pragma once
#include <ogl3d/math/orect.h>

// Thin wrapper around a native Win32 window plus its OpenGL render context.
class OWindow
{
public:
	OWindow();
	~OWindow();

	ORect getInnerSize(); // client-area size in pixels, used to set the GL viewport

	void makeCurrentContext();  // binds this window's GL context to the calling thread
	void present(bool vsync);   // swaps the front/back buffers to show the last frame
private:
	// Stored as void* so this header stays free of Windows.h / HWND / HGLRC
	void* m_handle = nullptr;  // HWND
	void* m_context = nullptr; // HGLRC, created via wglCreateContextAttribsARB
	void* m_hdc = nullptr;     // HDC, kept alive for the window's lifetime (CS_OWNDC)
};