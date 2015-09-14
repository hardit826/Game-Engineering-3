#ifndef EAE6320_MESH_H
#define EAE6320_MESH_H

// Header Files
//=============
#include "d3d9.h"
#include <gl/GLU.h>
#include <cstdint>
#include "../Windows/WindowsIncludes.h"

#ifdef EAE6320_PLATFORM_GL
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Mesh
		{

#if defined  EAE6320_PLATFORM_GL

			GLuint s_vertexArrayId;
			//Parameterized constructor for Mesh
		public:
			Mesh(GLuint i_vertexArrayId): s_vertexArrayId(i_vertexArrayId){}

#endif //EAE6320_PLATFORM_GL

#if defined EAE6320_PLATFORM_D3D
			//Parameterized constructor for Mesh
			IDirect3DVertexBuffer9* s_vertexBuffer;
			IDirect3DIndexBuffer9* s_indexBuffer;
			IDirect3DVertexDeclaration9* s_vertexDeclaration;
			IDirect3DDevice9* s_direct3dDevice;

		public:

			Mesh(IDirect3DVertexBuffer9* i_vertexBuffer, IDirect3DIndexBuffer9* i_indexBuffer, IDirect3DVertexDeclaration9* i_vertexDeclaration):
				s_vertexBuffer(i_vertexBuffer),
				s_indexBuffer(i_indexBuffer),
				s_vertexDeclaration(i_vertexDeclaration){}


#endif //EAE6320_PLATFORM_D3D
		
		public:
			struct sVertex
			{
				// POSITION
				// 2 floats == 8 bytes
				// Offset = 0
				float x, y;
				// COLOR0
				// 4 uint8_ts == 4 bytes
				// Offset = 8
				uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
			};

			sVertex* vertexData;

			sVertex* GetVertexData()
			{
				return vertexData;
			}

			void DrawMesh();

		};
	}
}

#endif	// EAE6320_MESH_H
