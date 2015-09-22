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

#include "../Windows/WindowsIncludes.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		bool Initialize( const HWND i_renderingWindow );
		void Render();
		bool ShutDown();
#ifdef EAE6320_PLATFORM_D3D
		IDirect3DDevice9* GetLocalDirect3dDevice();
#endif
	}

}

#endif	// EAE6320_GRAPHICS_H
