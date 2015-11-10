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
// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		//Creating 3 GameObjects for now , List will be better in future
		extern Renderable* o_cube;
		extern Renderable* o_floor;
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
