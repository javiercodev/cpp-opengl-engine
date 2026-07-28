#pragma once

// Thin wrapper around a native Win32 window
class OWindow
{
public:
	OWindow();
	~OWindow();

	void onDestroy();  // called from WndProc on WM_DESTROY, clears the handle
	bool isClosed();   // true once the underlying HWND has been destroyed
private:
	// Stored as void* so this header stays free of Windows.h / HWND
	void* m_handle = nullptr;
};