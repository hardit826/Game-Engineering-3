#include "../Graphics/GraphicEffect.h"
#include <cassert>
#include <d3dx9shader.h>
#include "../Math/Functions.h"
#include <sstream>
#include "../UserOutput/UserOutput.h"
#include "../Graphics/Graphics.h"
#include "../Math/cQuaternion.h"
#include "../Math/cMatrix_transformation.h"
#include<fstream>

#define Alpha_Transparency 0
#define Depth_Testing 1
#define Depth_Writing 2
#define Face_Culling 3

eae6320::Graphics::GraphicEffect::GraphicEffect(char* const i_path_effect)
{
	o_path_effect = i_path_effect;
	o_vertexShader = NULL;
	o_fragmentShader = NULL;
	s_direct3dDevice = Graphics::GetLocalDirect3dDevice();
	o_vertexShaderConstantTable = NULL;
	o_fragmentShaderConstantTable = NULL;
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

	SetRenderState();
}
void eae6320::Graphics::GraphicEffect::SetUniformHandle(UniformData i_uniformData, bool i_fromVertexShader)
{
	HRESULT result;
	ID3DXConstantTable * shaderConstantTable = i_fromVertexShader ? o_vertexShaderConstantTable:o_fragmentShaderConstantTable;
	result = shaderConstantTable->SetFloatArray(s_direct3dDevice, i_uniformData.uniformHandle, i_uniformData.values, i_uniformData.valueCountToSet);
	assert(SUCCEEDED(result));
}
D3DXHANDLE eae6320::Graphics::GraphicEffect::GetUniformHandle(const char* i_uniformName, bool i_fromVertexShader)
{
	ID3DXConstantTable* shaderConstantTable;
	if (i_fromVertexShader)
		return o_vertexShaderConstantTable->GetConstantByName(NULL, i_uniformName);
	else
		return o_fragmentShaderConstantTable->GetConstantByName(NULL, i_uniformName);

}
void eae6320::Graphics::GraphicEffect::SetDrawCallUniforms(eae6320::Math::cMatrix_transformation i_mvpMatrixTransformation,Camera i_camera)
{
	HRESULT result;
	g_transform_localToWorld = o_vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_localToWorld");
	g_transform_worldToView = o_vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_worldToView");
	g_transform_viewToScreen = o_vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_viewToScreen");

	const float aspectRatio = (float)800 / 600;
	Math::cVector cameraPosition = Math::cVector(0, 0, 10);
	Math::cQuaternion cameraRotation = Math::cQuaternion();

	const float z_nearPlane = 0.1f;
	const float z_farPlane = 100.0f;
	const float fieldOfView = Math::ConvertDegreesToRadians(60); //60 degree field of view

	Math::cMatrix_transformation g_matrix_worldToView = Math::cMatrix_transformation::cMatrix_transformation::
		CreateWorldToViewTransform(i_camera.camRotation, i_camera.camPosition);

	Math::cMatrix_transformation g_matrix_viewToScreen = Math::cMatrix_transformation::cMatrix_transformation::
		CreateViewToScreenTransform(fieldOfView, aspectRatio, z_nearPlane, z_farPlane);


	result = o_vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, g_transform_localToWorld,
		reinterpret_cast<const D3DXMATRIX*>(&i_mvpMatrixTransformation));
	assert(SUCCEEDED(result));

	result = o_vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, g_transform_worldToView,
		reinterpret_cast<const D3DXMATRIX*>(&g_matrix_worldToView));
	assert(SUCCEEDED(result));

	result = o_vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, g_transform_viewToScreen,
		reinterpret_cast<const D3DXMATRIX*>(&g_matrix_viewToScreen));
	assert(SUCCEEDED(result));
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

void eae6320::Graphics::GraphicEffect::SetRenderState()
{
	//Alpha_Transparency 
	if ((*render_state_value >> Alpha_Transparency) & 1)
	{
		s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		s_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		s_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{
		s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	//Depth_Testing 
	if ((*render_state_value >> Depth_Testing) & 1)
	{
		s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		s_direct3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	else
	{
		s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}
	//Depth_Writing 
	if ((*render_state_value >> Depth_Writing) & 1)
	{
		s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	else
	{
		s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	//Face_Culling 
	if ((*render_state_value >> Face_Culling) & 1)
	{
		s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	else
	{
		s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
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

