#include <ogl3d/window/owindow.h>
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
		window->onDestroy();
		break;
	}

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}

OWindow::OWindow()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"OGL3DWindow";
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // avoids uninitialized/black repaint areas

	assert(RegisterClassEx(&wc));

	// Standard overlapped window, minus the maximize button (fixed-size window)
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;

	// Client area size we actually want (1024x768); AdjustWindowRect grows the
	// rect to account for borders/title bar so the client area matches this
	RECT rect = { 0, 0, 1024, 768 };
	AdjustWindowRect(&rect, style, false);

	m_handle = CreateWindowEx(0, L"OGL3DWindow", L"C++ OpenGL Game Engine", style,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, NULL, NULL);

	assert(m_handle);

	// Stash "this" on the HWND so WndProc can map back to this instance
	SetWindowLongPtr((HWND)m_handle, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_handle, SW_SHOW);
	UpdateWindow((HWND)m_handle);
}

OWindow::~OWindow()
{
	DestroyWindow((HWND)m_handle);
}

void OWindow::onDestroy()
{
	m_handle = nullptr;
}

bool OWindow::isClosed()
{
	return m_handle == nullptr;
}