#pragma once
#include <ogl3d/math/ovec4.h>

// Owns the OpenGL context creation and exposes basic render commands.
// Construction sets up a throwaway "dummy" window/context just to load
// the WGL extensions (see ographicsengine.cpp) — the real rendering
// context lives on OWindow, not here.
class OGraphicsEngine
{
public:
	OGraphicsEngine();
	~OGraphicsEngine();

public:
	void clear(const Ovec4& color);
};