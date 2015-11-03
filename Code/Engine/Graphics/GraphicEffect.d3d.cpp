#include "../Graphics/GraphicEffect.h"
#include <cassert>
#include <d3dx9shader.h>
#include <sstream>
#include "../UserOutput/UserOutput.h"
#include "../Graphics/Graphics.h"
#include<fstream>
eae6320::Graphics::GraphicEffect::GraphicEffect(char* const i_path_effect)
{
	o_path_effect = i_path_effect;
	o_vertexShader = NULL;
	o_fragmentShader = NULL;
	o_vertexShaderPath = NULL;
	o_fragmentShaderPath = NULL;
	s_direct3dDevice = Graphics::GetLocalDirect3dDevice();
	o_vertexShaderConstantTable = NULL;
	o_fragmentShaderConstantTable = NULL;
	//o_bufferShader = NULL;
}

bool eae6320::Graphics::GraphicEffect::LoadShaders()
{
	if (!ReadFromBinEffectFile())
	{
		return false;
	}
	if (!LoadVertexShader())
	{
		return false;
	}
	if (!LoadFragmentShader())
	{
		return false;
	}

	return true;
}

void eae6320::Graphics::GraphicEffect::SetPath()
{
	HRESULT result;

	result = Graphics::GetLocalDirect3dDevice()->SetVertexShader(o_vertexShader);
	assert(SUCCEEDED(result));
	result = Graphics::GetLocalDirect3dDevice()->SetPixelShader(o_fragmentShader);
	assert(SUCCEEDED(result));
}

void eae6320::Graphics::GraphicEffect::SetDrawCallUniforms(eae6320::Math::cVector i_offset)
{
	HRESULT result;
	{
		float floatCount = 2;
		float floatArray[] = { i_offset.x , i_offset.y };
		result = o_vertexShaderConstantTable->SetFloatArray(eae6320::Graphics::GetLocalDirect3dDevice(),o_d3dHandle,floatArray,floatCount);
		assert(SUCCEEDED(result));
	}
}

bool eae6320::Graphics::GraphicEffect::LoadVertexShader()
{
	// Load the source code from file and compile it
	if (!ReadFromBinaryShaderFile(o_vertexShaderPath))
	{
		return false;
	}
	D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(o_bufferShader),&o_vertexShaderConstantTable);
	//ID3DXBuffer* compiledShader;
	//{
	//	//const char* sourceCodeFileName = "data/vertex.shader";
	//	const D3DXMACRO defines[] =
	//	{
	//		{ "EAE6320_PLATFORM_D3D", "1" },
	//		{ NULL, NULL }
	//	};
	//	ID3DXInclude* noIncludes = NULL;
	//	const char* entryPoint = "main";
	//	const char* profile = "vs_3_0";
	//	const DWORD noFlags = 0;
	//	ID3DXBuffer* errorMessages = NULL;
	//	//ID3DXConstantTable** noConstants = NULL;
	//	HRESULT result = D3DXCompileShaderFromFile(o_vertexShaderPath, defines, noIncludes, entryPoint, profile, noFlags,
	//		&compiledShader, &errorMessages, &o_vertexShaderConstantTable);
	o_d3dHandle = o_vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");
	// Create the vertex shader object
	bool wereThereErrors = false;
	{
		HRESULT result = s_direct3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(o_bufferShader),
			&o_vertexShader);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
			wereThereErrors = true;
		}
		delete[] o_bufferShader;
	}
	return !wereThereErrors;
}

bool eae6320::Graphics::GraphicEffect::LoadFragmentShader()
{
	//// Load the source code from file and compile it
	//ID3DXBuffer* compiledShader;
	//{
	//	//const char* sourceCodeFileName = "data/fragment.shader";
	//	const D3DXMACRO defines[] =
	//	{
	//		{ "EAE6320_PLATFORM_D3D", "1" },
	//		{ NULL, NULL }
	//	};
	//	ID3DXInclude* noIncludes = NULL;
	//	const char* entryPoint = "main";
	//	const char* profile = "ps_3_0";
	//	const DWORD noFlags = 0;
	//	ID3DXBuffer* errorMessages = NULL;
	//	ID3DXConstantTable** noConstants = NULL;
	//	HRESULT result = D3DXCompileShaderFromFile(o_fragmentShaderPath, defines, noIncludes, entryPoint, profile, noFlags,
	//		&compiledShader, &errorMessages, noConstants);
	if (!ReadFromBinaryShaderFile(o_fragmentShaderPath))
	{
		return false;
	}
	D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(o_bufferShader), &o_fragmentShaderConstantTable);

	bool wereThereErrors = false;
	{
		HRESULT result = s_direct3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(o_bufferShader),
			&o_fragmentShader);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
			wereThereErrors = true;
		}
		delete[] o_bufferShader;
	}
	return !wereThereErrors;
}

void eae6320::Graphics::GraphicEffect::ReleaseEffect()
{
	if (o_vertexShader)
	{
		o_vertexShader->Release();
		o_vertexShader = NULL;
	}
	if (o_fragmentShader)
	{
		o_fragmentShader->Release();
		o_fragmentShader = NULL;
	}
	if (o_vertexShaderConstantTable)
	{
		o_vertexShaderConstantTable->Release();
		o_vertexShaderConstantTable = NULL;
	}
	if (o_fragmentShaderConstantTable)
	{
		o_fragmentShaderConstantTable->Release();
		o_fragmentShaderConstantTable = NULL;
	}
}

bool eae6320::Graphics::GraphicEffect::ReadFromBinaryShaderFile(char* i_path_shader)
{
	std::ifstream ipBinShaderFile(i_path_shader, std::istream::binary);

	if (ipBinShaderFile)
	{
		//get file length
		ipBinShaderFile.seekg(0, ipBinShaderFile.end);
		unsigned int fileLen = ipBinShaderFile.tellg();
		ipBinShaderFile.seekg(0, ipBinShaderFile.beg);

		//read & store in buffer
		o_bufferShader = new char[fileLen];
		ipBinShaderFile.read(o_bufferShader, fileLen);

		return true;
	}
	return false;
}

