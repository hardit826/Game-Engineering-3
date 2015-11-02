/*
	This is an example builder program that just copies the source file to the target path
	(as opposed to actually "building" it in some way so that the target is different than the source)
*/

#ifndef EAE6320_CEFFECTBUILDER_H
#define EAE6320_CEFFECTBUILDER_H

// Header Files
//=============

#include "../../External/Lua/Includes.h"
#include "../BuilderHelper/cbBuilder.h"

// Class Declaration
//==================

namespace eae6320
{
	class cEffectBuilder : public cbBuilder
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
			float x, y;
			uint8_t b, g, r, a;
		};
#endif
#ifdef EAE6320_PLATFORM_GL
		struct sVertex
		{
			float x, y;
			uint8_t r,g,b,a;
		};
#endif

		char* m_fragmentPath;
		char* m_vertexPath;

	private:
		bool LoadAsset(const char* i_path);
		bool LoadPath_Values(lua_State& io_luaState);
		bool CreateEffectBinaryFile();

	};
}

#endif	// EAE6320_CMESHBUILDER_H
