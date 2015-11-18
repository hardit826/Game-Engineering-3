#ifndef _GRAPHIC_EFFECT_H
#define _GRAPHIC_EFFECT_H

#include "../Math/cVector.h"
#include "Camera.h"
#if defined(EAE6320_PLATFORM_D3D)
#include <d3dx9shader.h>
#include <d3d9.h>
#elif defined(EAE6320_PLATFORM_GL)
#include "../Windows/WindowsIncludes.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#endif

namespace eae6320
{
	namespace Graphics
	{
		class GraphicEffect
		{
#if defined(EAE6320_PLATFORM_D3D)
		private:
			IDirect3DVertexShader9* o_vertexShader;
			IDirect3DPixelShader9* o_fragmentShader;
			D3DXHANDLE o_d3dHandle;
			ID3DXConstantTable* o_vertexShaderConstantTable;
			ID3DXConstantTable* o_fragmentShaderConstantTable;
			char* o_bufferShader;
			bool ReadFromBinaryShaderFile(char* i_path_shader);

			//Three handles for the Transform Matrices
			D3DXHANDLE g_transform_localToWorld;
			D3DXHANDLE g_transform_worldToView;
			D3DXHANDLE g_transform_viewToScreen;

			//constructor for GraphicEffect D3D
		public:
		
			IDirect3DDevice9* s_direct3dDevice;
			
		/*	GraphicEffect(IDirect3DVertexShader9* i_vertexShader, IDirect3DPixelShader9* i_fragmentShader):
				o_vertexShader(i_vertexShader),o_fragmentShader(i_fragmentShader){}*/

#elif defined(EAE6320_PLATFORM_GL)
		public:
			GLuint o_programID;
			GLuint o_uniformLocation;
			GLuint g_transform_localToWorld;
			GLuint g_transform_worldToView;
			GLuint g_transform_viewToScreen;
			//constructor for GraphicEffect GL
			//GraphicEffect(GLuint i_programID) : o_programID(i_programID){}



			struct sLogInfo
			{
				GLchar* memory;
				sLogInfo(const size_t i_size) { memory = reinterpret_cast<GLchar*>(malloc(i_size)); }
				~sLogInfo() { if (memory) free(memory); }
			};

			bool CreateProgram();
			bool LinkProgram();
			bool eae6320::Graphics::GraphicEffect::LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage);
		
#endif
		private:
			char* o_binReadBuffer;
			char* o_vertexShaderPath;
			char* o_fragmentShaderPath;
			bool LoadFragmentShader();
			bool LoadVertexShader();
			bool ReadFromBinEffectFile();
			void SetRenderState();
			char* o_path_effect;
			uint8_t* render_state_value;


		public:
			GraphicEffect(char* const i_path_effect);
			
			bool LoadShaders();
			void SetPath();
			void SetDrawCallUniforms(eae6320::Math::cMatrix_transformation i_mvpMatrixTransformation,Camera i_cam);
			void ReleaseEffect();
		};
	}
}

#endif _GRAPHIC_EFFECT_H