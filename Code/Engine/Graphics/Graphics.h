/*
	This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Header Files
//=============
#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#endif
#include "Renderable.h"
#include "../Windows/WindowsIncludes.h"
#include <list>
#include "Camera.h"
// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		extern Renderable* o_ceiling;
		extern Renderable* o_floor;
		extern Renderable* o_metal;
		extern Renderable* o_railing;
		extern Renderable* o_wall;
		extern Renderable* o_lambert2;
		extern Renderable* o_cement;

		extern Camera* o_cam;
		bool Initialize( const HWND i_renderingWindow );
		void Render();
		bool ShutDown();
#ifdef EAE6320_PLATFORM_D3D
		IDirect3DDevice9* GetLocalDirect3dDevice();
#endif
		bool Clear();
		bool BeginDraw();
		void DrawRenderableList();
		bool EndDraw();
		bool ShowBuffer();
	}

}

#endif	// EAE6320_GRAPHICS_H
