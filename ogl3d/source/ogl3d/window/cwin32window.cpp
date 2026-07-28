#include <ogl3d/window/owindow.h>
#include <ogl3d/game/ogame.h>
#include <glad/glad_wgl.h>
#include <glad/glad.h>
#include <assert.h>
#include <Windows.h>

// Static window procedure. GWLP_USERDATA is used to recover the OWindow
// instance associated with this HWND, since WndProc can't be a member function.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		OWindow* window = (OWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

OWindow::OWindow()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"OGL3DWindow";
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // avoids uninitialized/black repaint areas
	wc.style = CS_OWNDC;                     // private DC required for OpenGL
	wc.hInstance = GetModuleHandle(nullptr);

	auto classId = RegisterClassEx(&wc);
	assert(classId);

	// Standard overlapped window, minus the maximize button (fixed-size window)
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;

	// Client area size we actually want (1024x768); AdjustWindowRect grows the
	// rect to account for borders/title bar so the client area matches this
	RECT rect = { 0, 0, 1024, 768 };
	AdjustWindowRect(&rect, style, false);

	m_handle = CreateWindowEx(NULL, MAKEINTATOM(classId), L"C++ OpenGL Game Engine", style,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, wc.hInstance, NULL);

	assert(m_handle);

	// Stash "this" on the HWND so WndProc can map back to this instance
	SetWindowLongPtr((HWND)m_handle, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_handle, SW_SHOW);
	UpdateWindow((HWND)m_handle);

	// --- Creating the real OpenGL render context (GLAD/WGL already loaded
	// by OGraphicsEngine's dummy context at this point) ---

	m_hdc = GetDC((HWND)m_handle);
	assert(m_hdc);

	int pixelFormatAttribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	int pixelFormat = 0;
	UINT numFormats = 0;
	wglChoosePixelFormatARB((HDC)m_hdc, pixelFormatAttribs, nullptr, 1, &pixelFormat, &numFormats);

	assert(numFormats);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {};
	DescribePixelFormat((HDC)m_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor);
	SetPixelFormat((HDC)m_hdc, pixelFormat, &pixelFormatDescriptor);

	int openGLAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	m_context = wglCreateContextAttribsARB((HDC)m_hdc, 0, openGLAttribs);
	assert(m_context);

	wglMakeCurrent((HDC)m_hdc, (HGLRC)m_context);
}

OWindow::~OWindow()
{
	// Shutdown order matters here to avoid heap corruption / leaked GL resources:
	wglMakeCurrent(NULL, NULL);              // 1. Detach the context from any DC
	wglDeleteContext((HGLRC)(m_context));    // 2. Delete the GL context
	ReleaseDC((HWND)m_handle, (HDC)m_hdc);   // 3. Release the DC (safe even with CS_OWNDC)
	DestroyWindow((HWND)m_handle);           // 4. Destroy the window
}

void OWindow::makeCurrentContext()
{
	wglMakeCurrent((HDC)GetDC((HWND)m_handle), (HGLRC)m_context);
}

void OWindow::present(bool vsync)
{
	wglSwapIntervalEXT(vsync);
	wglSwapLayerBuffers((HDC)GetDC((HWND)m_handle), WGL_SWAP_MAIN_PLANE);
}