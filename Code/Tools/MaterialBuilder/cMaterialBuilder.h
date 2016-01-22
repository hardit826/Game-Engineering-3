/*
	This is an example builder program that just copies the source file to the target path
	(as opposed to actually "building" it in some way so that the target is different than the source)
*/

#ifndef EAE6320_CMaterialBUILDER_H
#define EAE6320_CMaterialBUILDER_H

// Header Files
//=============
#include<string>
#include "../../External/Lua/Includes.h"
#include "../BuilderHelper/cbBuilder.h"
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include<vector>

// Class Declaration
//==================

namespace eae6320
{
	class cMaterialBuilder : public cbBuilder
	{
		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build( const std::vector<std::string>& i_arguments );

typedef
#if defined( EAE6320_PLATFORM_D3D )
			// This is conceptually a D3DXHANDLE but is defined this way
			// so that external files aren't required to specify the #include path to the DirectX SDK
			const char*
#elif defined( EAE6320_PLATFORM_GL )
			GLint
#endif
			tUniformHandle;

	enum ShaderTypes
	{
		vertexShader,
		fragmentShader,
	};

	struct UniformData
	{
		tUniformHandle uniformHandle;
		ShaderTypes shaderType;
		float values[4];
		uint32_t valueCountToSet;
	};
	struct TextureData
	{
		char* uniformName;
		char* texturePath;
	};

	private:
		char* effectPath;
		UniformData* uniformData;
		uint32_t uniformCount;
		std::string * uniformName;
		//std::vector<std::string> uniforrmNames;
		TextureData * m_textureData;
		uint32_t m_textureCount;

	private:
		bool LoadAsset(const char* i_path);
		bool LoadEffect_Path_Values(lua_State& io_luaState);

		bool LoadUniformData_Names(lua_State& io_luaState, int i);
		bool LoadUniformData_ShaderType(lua_State& io_luaState, int i);
		bool LoadUniformData_Values(lua_State& io_luaState, int i);
		bool LoadUniformData(lua_State& io_luaState);
		bool LoadTextureData(lua_State& io_luaState);

		bool CreateMaterialBinaryFile();
	};
}

#endif	// EAE6320_CMESHBUILDER_H
