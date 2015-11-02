// Header Files
//=============

#include "cEffectBuilder.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include "../../Engine/Windows/WindowsFunctions.h"

// Interface
//==========

// Build
//------

bool eae6320::cEffectBuilder::Build( const std::vector<std::string>& )
{
	//bool wereThereErrors = false;

	//// Copy the source to the target
	//{
	//	const bool dontFailIfTargetAlreadyExists = false;
	//	const bool updateTheTargetFileTime = true;
	//	std::string errorMessage;
	//	if ( !CopyFile( m_path_source, m_path_target, dontFailIfTargetAlreadyExists, updateTheTargetFileTime, &errorMessage ) )
	//	{
	//		wereThereErrors = true;
	//		std::stringstream decoratedErrorMessage;
	//		decoratedErrorMessage << "Windows failed to copy \"" << m_path_source << "\" to \"" << m_path_target << "\": " << errorMessage;
	//		eae6320::OutputErrorMessage( decoratedErrorMessage.str().c_str(), __FILE__ );
	//	}
	//}
	//
	//return !wereThereErrors;

	if (!LoadAsset(m_path_source))
	{
		return false;
	}
	if (!CreateEffectBinaryFile())
	{
		return false;
	}
	return true;
}

bool eae6320::cEffectBuilder::LoadAsset(const char* i_path)
{
	bool wereThereErrors = false;

	// Create a new Lua state
	lua_State* luaState = NULL;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage("Failed to create a new Lua state");

			goto OnExit;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, i_path);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage(lua_tostring(luaState, -1));
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		const int argumentCount = 0;
		const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		const int noMessageHandler = 0;
		const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const int returnedValueCount = lua_gettop(luaState);
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					wereThereErrors = true;
					std::stringstream errMsg;
					//std::cerr << "Asset files must return a table (instead of a " <<
					//	luaL_typename(luaState, -1) << ")\n";
					// Pop the returned non-table value
					errMsg << "Asset files must return a table (instead of a " <<
							luaL_typename(luaState, -1) << ")\n";
					eae6320::OutputErrorMessage(errMsg.str().c_str());
					lua_pop(luaState, 1);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errMsg;
				//std::cerr << "Asset files must return a single table (instead of " <<
				//	returnedValueCount << " values)"
				//	"\n";
				// Pop every value that was returned
				errMsg << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)"
					"\n";
				eae6320::OutputErrorMessage(errMsg.str().c_str());
				lua_pop(luaState, returnedValueCount);
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage(lua_tostring(luaState, -1));
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	if (!LoadPath_Values(*luaState))
	{
		wereThereErrors = true;
	}

	// Pop the table
	lua_pop(luaState, 1);

OnExit:

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		assert(lua_gettop(luaState) == 0);

		lua_close(luaState);
		luaState = NULL;
	}

	return !wereThereErrors;
}

bool eae6320::cEffectBuilder::LoadPath_Values(lua_State& io_luaState)
{
	bool wereThereErrors = false;

	if (lua_isnil(&io_luaState, -1))
	{
		wereThereErrors = true;
		std::stringstream errorMessage;
		errorMessage << "Asset file must not return NULL.\n";
		eae6320::OutputErrorMessage(errorMessage.str().c_str());
		goto OnExit;
	}
	{
		const char* const key = "vertexShaderPath";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		if (!lua_isstring(&io_luaState, -1))
		{
			wereThereErrors = true;
			std::stringstream errMsg;
			//std::cerr << "Asset files must return a table (instead of a " <<
			//	luaL_typename(luaState, -1) << ")\n";
			// Pop the returned non-table value
			errMsg << "Asset files must return a string (instead of a " <<
				luaL_typename(&io_luaState, -1) << ")\n";
			eae6320::OutputErrorMessage(errMsg.str().c_str());
			lua_pop(&io_luaState, 1);
			goto OnExit;
		}

		char* tempBuffer = const_cast<char*>(lua_tostring(&io_luaState, -1));
		size_t bufferLen = std::strlen(tempBuffer) + 1;
		m_vertexPath = new char[bufferLen];
		strcpy_s(m_vertexPath, bufferLen, tempBuffer);
		m_vertexPath[bufferLen] = '\0';

		lua_pop(&io_luaState, 1);
	}
	{
		const char* const key = "fragmentShaderPath";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		if (!lua_isstring(&io_luaState, -1))
		{
			wereThereErrors = true;
			std::stringstream errMsg;
			//std::cerr << "Asset files must return a table (instead of a " <<
			//	luaL_typename(luaState, -1) << ")\n";
			// Pop the returned non-table value
			errMsg << "Asset files must return a string (instead of a " <<
				luaL_typename(&io_luaState, -1) << ")\n";
			eae6320::OutputErrorMessage(errMsg.str().c_str());
			lua_pop(&io_luaState, 1);
			goto OnExit;
		}

		char* tempBuffer = const_cast<char*>(lua_tostring(&io_luaState, -1));
		size_t bufferLen = std::strlen(tempBuffer) + 1;
		m_fragmentPath = new char[bufferLen];
		strcpy_s(m_fragmentPath, bufferLen, tempBuffer);
		m_fragmentPath[bufferLen] = '\0';

		lua_pop(&io_luaState, 1);
	}

OnExit:
	lua_pop(&io_luaState, 1);

	return !wereThereErrors;
}

bool eae6320::cEffectBuilder::CreateEffectBinaryFile()
{
	std::ofstream outputBinEffectFile(m_path_target, std::ofstream::binary);

	outputBinEffectFile.write(m_vertexPath, std::strlen(m_vertexPath));
	outputBinEffectFile.write("\0",1);
	
	outputBinEffectFile.write(m_fragmentPath, std::strlen(m_fragmentPath));
	outputBinEffectFile.write("\0", 1);

	outputBinEffectFile.close();

	return true;
}


