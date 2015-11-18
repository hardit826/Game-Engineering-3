#include "GraphicEffect.h"
#include<sstream>
#include<fstream>
#include<cassert>
#include "../UserOutput/UserOutput.h"

bool eae6320::Graphics::GraphicEffect::ReadFromBinEffectFile()
{
	std::ifstream ipBinEffectFile(o_path_effect, std::istream::binary);
	if (ipBinEffectFile)
	{
		//get file length
		ipBinEffectFile.seekg(0, ipBinEffectFile.end);
		unsigned int fileLen = ipBinEffectFile.tellg();
		ipBinEffectFile.seekg(0, ipBinEffectFile.beg);
		//read & store in buffer
		o_binReadBuffer = new char[fileLen];
		ipBinEffectFile.read(o_binReadBuffer, fileLen);
		//extract after reading binary
		if (ipBinEffectFile)
		{
			o_vertexShaderPath = o_binReadBuffer;
			o_fragmentShaderPath = o_binReadBuffer + std::strlen(o_vertexShaderPath) + 1;
			render_state_value = reinterpret_cast<uint8_t*>
				(o_binReadBuffer + std::strlen(o_vertexShaderPath) + 1 + std::strlen(o_fragmentShaderPath) + 1);
		}
		return true;
	}
	return false;
}