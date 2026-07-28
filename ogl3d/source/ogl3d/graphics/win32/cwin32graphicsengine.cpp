#include <ogl3d/graphics/ographicsengine.h>
#include <glad/glad_wgl.h>
#include <glad/glad.h>
#include <assert.h>
#include <stdexcept>

// Win32-specific half of OGraphicsEngine: loads the WGL extension functions
// (wglChoosePixelFormatARB, wglCreateContextAttribsARB, etc.) that OWindow
// needs to create its real rendering context.
//
// GLAD can't load these until *some* GL context is current, so this
// constructor creates a throwaway "dummy" window + legacy context just
// long enough to call gladLoadWGL/gladLoadGL, then tears it all down.

OGraphicsEngine::OGraphicsEngine()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"OGL3DDummyWindow";
	wc.lpfnWndProc = DefWindowProc;
	wc.style = CS_OWNDC; // required for a window to own an OpenGL-capable DC
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // avoids uninitialized/black repaint areas

	auto classId = RegisterClassEx(&wc);
	assert(classId);

	// Standard overlapped window, minus the maximize button (fixed-size window)
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;

	auto dummyWindow = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);

	assert(dummyWindow);

	auto dummyDC = GetDC(dummyWindow);

	// Minimal legacy pixel format, just enough to get a GL context bound
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {};
	pixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.cColorBits = 32;
	pixelFormatDescriptor.cAlphaBits = 8;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cStencilBits = 8;
	pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

	auto pixelFormat = ChoosePixelFormat(dummyDC, &pixelFormatDescriptor);
	SetPixelFormat(dummyDC, pixelFormat, &pixelFormatDescriptor);

	auto dummyContext = wglCreateContext(dummyDC);
	assert(dummyContext);

	wglMakeCurrent(dummyDC, dummyContext);

	// Now that a context is current, GLAD can resolve the WGL/GL function pointers
	if (!gladLoadWGL(dummyDC))
		throw std::runtime_error("Failed to initialize OpenGL context");

	if (!gladLoadGL())
		throw std::runtime_error("Failed to initialize OpenGL context");

	// Tear down the dummy context/window; the real one gets created by OWindow
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(dummyContext);
	ReleaseDC(dummyWindow, dummyDC);
	DestroyWindow(dummyWindow);
	UnregisterClass(MAKEINTATOM(classId), wc.hInstance);
}

OGraphicsEngine::~OGraphicsEngine()
{
}