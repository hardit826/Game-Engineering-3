#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef EAE6320_PLATFORM_D3D
#include <d3d9.h>
#include <d3dx9shader.h>
#endif // EAE6320_PLATFORM_D3D

#ifdef EAE6320_PLATFORM_GL
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#endif // EAE6320_PLATFORM_GL

namespace eae6320
{
	namespace Graphics
	{
		struct Texture
		{
#ifdef EAE6320_PLATFORM_D3D
		public:
			typedef IDirect3DTexture9* tTextureHandle;
			typedef DWORD tSampler2D;
#endif // EAE6320_PLATFORM_D3D

#ifdef EAE6320_PLATFORM_GL
		public:
			typedef GLuint tTextureHandle;
			typedef GLint tSampler2D;
#endif // EAE6320_PLATFORM_GL

		private:
			char * o_texturePath;
		public:
			tTextureHandle o_texture;
			tSampler2D m_samplerID;

		public:
			Texture() {};
			Texture(char* const i_path_texture);
			bool LoadTexture();
			void ReleaseTexture();
		};
	}
}

#endif // TEXTURE_H