/*
	This is an example builder program that just copies the source file to the target path
	(as opposed to actually "building" it in some way so that the target is different than the source)
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Header Files
//=============

#include "../../External/Lua/Includes.h"
#include "../BuilderHelper/cbBuilder.h"

// Class Declaration
//==================

namespace eae6320
{
	class cMeshBuilder : public cbBuilder
	{
		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build( const std::vector<std::string>& i_arguments );

	private:
#ifdef EAE6320_PLATFORM_D3D
		struct sVertex
		{
			float x, y ,z;
			uint8_t b, g, r, a;
		};
#endif
#ifdef EAE6320_PLATFORM_GL
		struct sVertex
		{
			float x, y ,z;
			uint8_t r,g,b,a;
		};
#endif

		uint32_t o_vertexCount;
		sVertex *o_vertexData;
		uint32_t o_indexCount;
		uint32_t *o_indexData;

	private:
		bool LoadAsset(const char* i_path);
		bool Table_Values(lua_State& io_luaState);
		bool Table_Vertices(lua_State& io_luaState);
		bool Table_Vertices_Values(lua_State& io_luaState);
		bool Table_Vertices_Pos(lua_State& io_luaState, int i_vertexIndex);
		bool Table_Vertices_Color(lua_State& io_luaState, int i_vertexIndex);
		bool Table_Indices(lua_State& io_luaState);
		bool Table_Indices_Values(lua_State& io_luaState);
		bool CreateBinaryFile();

	};
}

#endif	// EAE6320_CMESHBUILDER_H
