#ifndef GUARD_MATERIAL_H
#define GUARD_MATERIAL_H
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "GraphicEffect.h"
#include<vector>
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

		public:
			explicit Material(){}
			explicit Material(char* const i_path_material);
				
		private:
			char * o_path_material;
			char * o_path_effect;
			uint32_t uniformCount;
			std::string * uniformNames;
			GraphicEffect::UniformData* uniformData;
			GraphicEffect* effect;
			char* o_binReadBuffer;
		public:
			bool ReadFromBinMaterialFile();
			bool GetUniformHandleForMaterial();
			bool LoadMaterial();
			void SetUniformDataMaterial();
			void SetUniformDataEngine(Math::cMatrix_transformation i_localToWorld, Camera i_camera);
		

		};

	}
}

#endif // !GUARD_MATERIAL_H
