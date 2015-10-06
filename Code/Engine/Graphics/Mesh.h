#ifndef EAE6320_MESH_H
#define EAE6320_MESH_H

// Header Files
//=============
//Made the platform preprocessor for Direct3D as mentioned by John Paul and Duncan
#ifdef EAE6320_PLATFORM_D3D
#include "d3d9.h"
#endif
//#include <gl/GLU.h>

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
			IDirect3DVertexBuffer9* m_vertexBuffer;
			IDirect3DIndexBuffer9* m_indexBuffer;
			IDirect3DVertexDeclaration9* m_vertexDeclaration;
			

		public:
			IDirect3DDevice9* s_direct3dDevice;
			Mesh(IDirect3DVertexBuffer9* i_vertexBuffer, IDirect3DIndexBuffer9* i_indexBuffer, IDirect3DVertexDeclaration9* i_vertexDeclaration):
				m_vertexBuffer(i_vertexBuffer),
				m_indexBuffer(i_indexBuffer),
				m_vertexDeclaration(i_vertexDeclaration){}


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

//Seperate platform for OpenGl and Direct3D as mentioned by John Paul.
#if defined EAE6320_PLATFORM_D3D
				uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#elif defined EAE6320_PLATFORM_GL
				uint8_t r, g, b, a;
#endif

			};

			//sVertex* vertexData;

			//sVertex* GetVertexData()
			//{
			//	return vertexData;
			//}

			bool LoadMesh(const char* const i_path);
			void DrawMesh();

			bool CreateIndexBuffer();
			bool CreateVertexBuffer();
			HRESULT GetVertexProcessingUsage(DWORD& o_usage);

			unsigned int m_vertexCount;
			unsigned int m_indexCount;
			sVertex *m_vertexData;

			uint32_t *m_indexData;

			char * m_tempBinReadBuffer;

		private:
			bool ReadBinMeshFile(const char* const i_path);
			bool LoadGraphicsMeshData();
			
		};
	}
}

#endif	// EAE6320_MESH_H
