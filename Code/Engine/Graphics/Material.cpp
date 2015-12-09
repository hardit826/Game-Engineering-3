#include "Material.h"
#include<sstream>
#include<fstream>
#include<cassert>
#include "../UserOutput/UserOutput.h"
#include "GraphicEffect.h"
#include "../Math/cMatrix_transformation.h"
eae6320::Graphics::Material::Material(char* const i_path_material)
{
	//s_direct3dDevice = i_direct3dDevice;
	o_path_material = i_path_material;
	ReadFromBinMaterialFile();
	effect = new GraphicEffect(o_path_effect);
}
bool eae6320::Graphics::Material::GetUniformHandleForMaterial()
{
	for (auto i = 0; i < uniformCount; ++i)
	{
		int bVertexShader = 1;

		if (uniformData[i].shaderType == GraphicEffect::ShaderTypes::vertexShader)
			bVertexShader = 1;
		if (uniformData[i].shaderType == GraphicEffect::ShaderTypes::fragmentShader)
			bVertexShader = 0;

		uniformData[i].uniformHandle = effect->GetUniformHandle(bVertexShader, uniformNames[i].c_str());
	}
	return true;
}
bool eae6320::Graphics::Material::LoadMaterial()
{

	if (!effect->LoadShaders())
	{
		return false;
	}
	if (!GetUniformHandleForMaterial())
	{
		return false;
	}

	return true;
}
void eae6320::Graphics::Material::SetUniformDataMaterial()
{
	//Set the effect path
	effect->SetPath();
	
	for (size_t i = 0; i < uniformCount; ++i)
	{
		int bVertexShader = 1;

		if (uniformData[i].shaderType == GraphicEffect::ShaderTypes::vertexShader)
			bVertexShader = 1;
		else if (uniformData[i].shaderType == GraphicEffect::ShaderTypes::fragmentShader)
			bVertexShader = 0;

		effect->SetUniformHandle(bVertexShader, uniformData[i]);
	}
}
void eae6320::Graphics::Material::SetUniformDataEngine(Math::cMatrix_transformation i_localToWorld, Camera i_camera)
{
	effect->SetDrawCallUniforms(i_localToWorld,i_camera);
}
bool eae6320::Graphics::Material::ReadFromBinMaterialFile()
{
	std::ifstream ipBinEffectFile(o_path_material, std::istream::binary);
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
			o_path_effect = o_binReadBuffer;
			auto pathLengthNullTerm = o_binReadBuffer + std::strlen(o_path_effect) + 1;
			uniformCount = *reinterpret_cast<uint32_t*>(pathLengthNullTerm);
			auto path_length = std::strlen(o_path_effect);

			uniformData = reinterpret_cast<GraphicEffect::UniformData*>(pathLengthNullTerm + sizeof(uint32_t));
			uniformNames = new std::string[uniformCount];

			size_t previousStringSize =0;
			for (size_t i = 0; i < uniformCount; ++i)
			{
				auto totalStructSize = sizeof(GraphicEffect::UniformData);
				uniformNames[i] = pathLengthNullTerm + sizeof(uint32_t) + totalStructSize * uniformCount + previousStringSize;
				previousStringSize = uniformNames[i].size() + 1;
			}
		}
		return true;
	}
	return false;
}
