#include "../Graphics/GraphicEffect.h"
#include <cassert>
#include <d3dx9shader.h>
#include <sstream>
#include "../UserOutput/UserOutput.h"
#include "../Graphics/Graphics.h"

eae6320::Graphics::GraphicEffect::GraphicEffect(char* const i_path_effect)
{
	o_path_effect = i_path_effect;
	

	o_vertexShader = NULL;
	o_fragmentShader = NULL;
	o_vertexShaderPath = NULL;
	o_fragmentShaderPath = NULL;
	s_direct3dDevice = Graphics::GetLocalDirect3dDevice();
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
	ID3DXBuffer* compiledShader;
	{
		//const char* sourceCodeFileName = "data/vertex.shader";
		const D3DXMACRO defines[] =
		{
			{ "EAE6320_PLATFORM_D3D", "1" },
			{ NULL, NULL }
		};
		ID3DXInclude* noIncludes = NULL;
		const char* entryPoint = "main";
		const char* profile = "vs_3_0";
		const DWORD noFlags = 0;
		ID3DXBuffer* errorMessages = NULL;
		//ID3DXConstantTable** noConstants = NULL;
		HRESULT result = D3DXCompileShaderFromFile(o_vertexShaderPath, defines, noIncludes, entryPoint, profile, noFlags,
			&compiledShader, &errorMessages, &o_vertexShaderConstantTable);

		o_d3dHandle = o_vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");


		if (SUCCEEDED(result))
		{
			if (errorMessages)
			{
				errorMessages->Release();
			}
		}
		else
		{
			if (errorMessages)
			{
				std::stringstream errorMessage;
				errorMessage << "Direct3D failed to compile the vertex shader from the file " << o_vertexShaderPath
					<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
				eae6320::UserOutput::Print(errorMessage.str());
				errorMessages->Release();
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "Direct3D failed to compile the vertex shader from the file " << o_vertexShaderPath;
				eae6320::UserOutput::Print(errorMessage.str());
			}
			return false;
		}
	}
	// Create the vertex shader object
	bool wereThereErrors = false;
	{
		HRESULT result = s_direct3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
			&o_vertexShader);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
			wereThereErrors = true;
		}
		compiledShader->Release();
	}
	return !wereThereErrors;
}

bool eae6320::Graphics::GraphicEffect::LoadFragmentShader()
{
	// Load the source code from file and compile it
	ID3DXBuffer* compiledShader;
	{
		//const char* sourceCodeFileName = "data/fragment.shader";
		const D3DXMACRO defines[] =
		{
			{ "EAE6320_PLATFORM_D3D", "1" },
			{ NULL, NULL }
		};
		ID3DXInclude* noIncludes = NULL;
		const char* entryPoint = "main";
		const char* profile = "ps_3_0";
		const DWORD noFlags = 0;
		ID3DXBuffer* errorMessages = NULL;
		ID3DXConstantTable** noConstants = NULL;
		HRESULT result = D3DXCompileShaderFromFile(o_fragmentShaderPath, defines, noIncludes, entryPoint, profile, noFlags,
			&compiledShader, &errorMessages, noConstants);
		if (SUCCEEDED(result))
		{
			if (errorMessages)
			{
				errorMessages->Release();
			}
		}
		else
		{
			if (errorMessages)
			{
				std::stringstream errorMessage;
				errorMessage << "Direct3D failed to compile the fragment shader from the file " << o_fragmentShaderPath
					<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
				eae6320::UserOutput::Print(errorMessage.str());
				errorMessages->Release();
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "Direct3D failed to compile the fragment shader from the file " << o_fragmentShaderPath;
				eae6320::UserOutput::Print(errorMessage.str());
			}
			return false;
		}
	}
	// Create the fragment shader object
	bool wereThereErrors = false;
	{
		HRESULT result = s_direct3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
			&o_fragmentShader);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
			wereThereErrors = true;
		}
		compiledShader->Release();
	}
	return !wereThereErrors;
}

void eae6320::Graphics::GraphicEffect::ReleaseEffect()
{
	if (o_fragmentShader)
	{
		o_fragmentShader->Release();
		o_fragmentShader = NULL;
	}
	if (o_vertexShader)
	{
		o_vertexShader->Release();
		o_vertexShader = NULL;
	}
}

