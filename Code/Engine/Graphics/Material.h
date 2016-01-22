#ifndef GUARD_MATERIAL_H
#define GUARD_MATERIAL_H
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "GraphicEffect.h"
#include<vector>
#include "../Math/cMatrix_transformation.h"
#include "Texture.h"
namespace eae6320
{
	namespace Graphics
	{
		struct Material
		{

#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DDevice9* s_direct3dDevice;
			// This is conceptually a D3DXHANDLE but is defined this way
			// so that external files aren't required to specify the #include path to the DirectX SDK
			typedef const char*
#elif defined( EAE6320_PLATFORM_GL )
			typedef GLint
#endif
				tUniformHandle;
			
		private:
			char * o_path_material,* o_path_effect;
			std::string * uniformNames;
			uint32_t uniformCount;
			GraphicEffect::UniformData* uniformData;
			GraphicEffect* effect;
			char* o_binReadBuffer;

			Texture *m_textureList;
			uint32_t m_textureCount;

			struct TextureData
			{
				char* uniformName;
				char* texturePath;
			};
			TextureData *m_textureData;

		public:
			explicit Material() {}
			explicit Material(char* const o_path_material);
			bool ReadFromBinMaterialFile();
			bool GetUniformHandleForMaterial();	
			void SetUniformDataMaterial();
			void SetUniformDataEngine(Math::cMatrix_transformation i_localToWorld, Camera i_camera);
			bool LoadMaterial();
		

		};

	}
}

#endif // !GUARD_MATERIAL_H
