// Header Files
//=============

#include "cMaterialBuilder.h"
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include "../../Engine/Windows/WindowsFunctions.h"

// Interface
//==========

// Build
//------

bool eae6320::cMaterialBuilder::Build( const std::vector<std::string>& )
{

	if (!LoadAsset(m_path_source))
	{
		return false;
	}
	if (!CreateMaterialBinaryFile())
	{
		return false;
	}
	return true;
}

bool eae6320::cMaterialBuilder::LoadAsset(const char* i_path)
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
	if (!LoadEffect_Path_Values(*luaState))
	{
		wereThereErrors = true;
	}
	if (!LoadUniformData(*luaState))
	{
		wereThereErrors;
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

bool eae6320::cMaterialBuilder::LoadEffect_Path_Values(lua_State& io_luaState)
{
	bool wereThereErrors = false;

	{
		const char* const key = "pathEffect";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		if (!lua_isstring(&io_luaState, -1))
		{
			lua_pop(&io_luaState, 1);
			wereThereErrors = true;
			std::stringstream errMsg;
			//std::cerr << "Asset files must return a table (instead of a " <<
			//	luaL_typename(luaState, -1) << ")\n";
			// Pop the returned non-table value
			errMsg << "Asset files must return a string (instead of a " <<
				luaL_typename(&io_luaState, -1) << ")\n";
			eae6320::OutputErrorMessage(errMsg.str().c_str());
			goto OnExit;
		}

		char* tempBuffer = const_cast<char*>(lua_tostring(&io_luaState, -1));
		size_t bufferLen = std::strlen(tempBuffer) + 1;
		effectPath = new char[bufferLen];
		strcpy_s(effectPath, bufferLen, tempBuffer);
		effectPath[bufferLen] = '\0';

		lua_pop(&io_luaState, 1);
	}

OnExit:

	return !wereThereErrors;
}

bool eae6320::cMaterialBuilder::LoadUniformData_Names(lua_State& io_luaState, int i)
{
	
	const char* const key = "name";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	uniformName[i - 1] = const_cast<char*>(lua_tostring(&io_luaState, -1));
	lua_pop(&io_luaState, 1);
	
	return true;
}

bool eae6320::cMaterialBuilder::LoadUniformData_ShaderType(lua_State& io_luaState, int i)
{
	
	const char* const key = "shaderType";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	std::string shaderTypeName = lua_tostring(&io_luaState, -1);
	if (shaderTypeName.compare("vertex") == 0)
		uniformData[i - 1].shaderType = ShaderTypes::vertexShader;
	else
		uniformData[i - 1].shaderType = ShaderTypes::fragmentShader;
	lua_pop(&io_luaState, 1);
	
	return true;
}

bool eae6320::cMaterialBuilder::LoadUniformData_Values(lua_State& io_luaState, int i)
{
	
	const char* const key = "values";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	uniformData[i - 1].valueCountToSet = luaL_len(&io_luaState, -1);
		
	for (unsigned int j = 1; j <= uniformData[i - 1].valueCountToSet; ++j)
	{
		lua_pushinteger(&io_luaState, j);
		lua_gettable(&io_luaState, -2);
		uniformData[i - 1].values[j - 1] = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
	}
	lua_pop(&io_luaState, 1);
	
	return true;
}

bool eae6320::cMaterialBuilder::LoadUniformData(lua_State& io_luaState)
{

	bool wereThereErrors = false;
	{
		const char* const key = "uniform_data";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		uniformCount = luaL_len(&io_luaState, -1);

		uniformName = new std::string[uniformCount];
		uniformData = new eae6320::cMaterialBuilder::UniformData[uniformCount];

		for (auto i = 1; i <= uniformCount; ++i)
		{

			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);

			if (lua_istable(&io_luaState, -1))
			{
				LoadUniformData_Names(io_luaState, i);
				LoadUniformData_ShaderType(io_luaState, i);
				LoadUniformData_Values(io_luaState, i);
			}
			lua_pop(&io_luaState, 1);
		}
		lua_pop(&io_luaState, 1);
	}

OnExit:
	return !wereThereErrors;
}

bool eae6320::cMaterialBuilder::CreateMaterialBinaryFile()
{
	std::ofstream outputBinMaterialFile(m_path_target, std::ofstream::binary);

	outputBinMaterialFile.write(effectPath, std::strlen(effectPath));
	outputBinMaterialFile.write("\0",1);
	auto totalUniformStructSize = sizeof(UniformData) * uniformCount;

	char* o_uniformCount = reinterpret_cast<char*>(&uniformCount);
	char* o_uniformData = reinterpret_cast<char*>(uniformData);

	outputBinMaterialFile.write(o_uniformCount, sizeof(uint32_t));
	outputBinMaterialFile.write(o_uniformData, totalUniformStructSize);

	for (size_t i = 0; i < uniformCount; ++i)
	{
		outputBinMaterialFile.write(uniformName[i].c_str(), uniformName[i].size());
		outputBinMaterialFile.write("\0", 1);
	}

	outputBinMaterialFile.close();

	return true;
}
