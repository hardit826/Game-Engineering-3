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
bool eae6320::Graphics::Material::LoadMaterial()
{
	if (!effect->LoadShaders())
	{
		return false;
	}
	for (auto i = 0; i <= uniformCount; ++i)
	{
		bool fromVertexShader = true;

		if (uniformData[i].shaderType == GraphicEffect::ShaderTypes::fragmentShader)
			fromVertexShader = false;

		uniformData[i].uniformHandle = effect->GetUniformHandle(uniformNames[i].c_str(), fromVertexShader);
	}
	return true;
}
bool eae6320::Graphics::Material::SetMaterial()
{
	effect->SetPath();
	return true;
}
void eae6320::Graphics::Material::SetMaterialUniform()
{
	for (auto i = 0; i < uniformCount; ++i)
	{
		bool fromVertexShader = true;

		if (uniformData[i].shaderType == GraphicEffect::ShaderTypes::fragmentShader)
			fromVertexShader = false;

		effect->SetUniformHandle(uniformData[i], fromVertexShader);
	}

}
void eae6320::Graphics::Material::SetEngineUniforms(Math::cMatrix_transformation i_localToWorld, Camera i_camera)
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
			uniformCount = *reinterpret_cast<uint32_t*>(o_binReadBuffer + std::strlen(o_path_effect) + 1);
			auto path_length = std::strlen(o_path_effect);
			uniformData = reinterpret_cast<GraphicEffect::UniformData*>(o_binReadBuffer + path_length + 1 + sizeof(uint32_t));
			uniformNames = new std::string[uniformCount];
			
			unsigned int previousStringSize = 0;
			for (unsigned int i = 0; i < uniformCount; ++i)
			{
				auto totalStructSize = sizeof(GraphicEffect::UniformData);
				uniformNames[i] = o_binReadBuffer + path_length + 1 + sizeof(uint32_t) + totalStructSize * uniformCount + previousStringSize;
				previousStringSize = uniformNames[i].size() + 1;
			}
			
		}
		return true;
	}
	return false;
}