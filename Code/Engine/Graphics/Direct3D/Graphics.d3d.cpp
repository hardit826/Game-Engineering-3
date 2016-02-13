// Header Files
//=============

#include "../Graphics.h"
#include "../GraphicEffect.h"
#include <cassert>
#include <cstdint>
#include <d3d9.h>
#include <d3dx9shader.h>
#include <sstream>
#include "../../UserOutput/UserOutput.h"
#include "../Mesh.h"
#include <list>
#include "../Material.h"

// Static Data Initialization
//===========================

eae6320::Graphics::Renderable* eae6320::Graphics::o_ceiling = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_floor = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_metal = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_railing = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_lambert2 = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_wall = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_cement = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_debugCylinder1 = NULL;

eae6320::Graphics::Camera* eae6320::Graphics::o_cam = NULL;
namespace
{
	
	HWND s_renderingWindow = NULL;
	IDirect3D9* s_direct3dInterface = NULL;
	IDirect3DDevice9* s_direct3dDevice = NULL;

	//eae6320::Graphics::GraphicEffect* s_effect = NULL;
	//eae6320::Graphics::GraphicEffect* s_effect_transparent = NULL;

	eae6320::Graphics::Material * s_material_ceiling;
	eae6320::Graphics::Material * s_material_floor;
	eae6320::Graphics::Material * s_material_metal;
	eae6320::Graphics::Material * s_material_railing;
	eae6320::Graphics::Material * s_material_wall;
	eae6320::Graphics::Material * s_material_cement;


	// This struct determines the layout of the data that the CPU will send to the GPU
	//struct sVertex
	//{
	//	// POSITION
	//	// 2 floats == 8 bytes
	//	// Offset = 0
	//	float x, y;
	//	// COLOR0
	//	// 4 uint8_ts == 4 bytes
	//	// Offset = 8
	//	uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
	//};
	//IDirect3DVertexDeclaration9* s_vertexDeclaration = NULL;

	// The vertex buffer holds the data for each vertex
	//IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
	// An index buffer describes how to make triangles with the vertices
	// (i.e. it defines the vertex connectivity)
	//IDirect3DIndexBuffer9* s_indexBuffer = NULL;

	eae6320::Graphics::Mesh *s_ceiling = NULL;
	eae6320::Graphics::Mesh *s_floor = NULL;
	eae6320::Graphics::Mesh *s_metal = NULL;
	eae6320::Graphics::Mesh *s_railing = NULL;
	eae6320::Graphics::Mesh *s_lambert2 = NULL;
	eae6320::Graphics::Mesh *s_wall = NULL;
	eae6320::Graphics::Mesh *s_cement = NULL;
	eae6320::Graphics::Mesh *s_debugCylinder1 = NULL;
	//eae6320::Graphics::Mesh *s_box_2 = NULL;
	//eae6320::Graphics::Mesh *s_box_3 = NULL;
	//eae6320::Graphics::Mesh *s_box_4 = NULL;
	//eae6320::Graphics::Mesh *s_bigbox = NULL;


	// The vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want
	IDirect3DVertexShader9* s_vertexShader = NULL;
	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	IDirect3DPixelShader9* s_fragmentShader = NULL;
}



// Helper Function Declarations
//=============================

namespace
{
	bool CreateDevice();
	//bool CreateIndexBuffer();
	bool CreateInterface();
	//bool CreateVertexBuffer();
	//HRESULT GetVertexProcessingUsage( DWORD& o_usage );
	//bool LoadFragmentShader();
	//bool LoadVertexShader();
}

// Interface
//==========

//IDirect3DDevice9* eae6320::Graphics::GetLocalDirect3dDevice()
//{
//	return s_direct3dDevice;
//}

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	s_renderingWindow = i_renderingWindow;
	
	//s_material = new Material("data/materialBlueTransparent.lua");

	o_cam = new Camera();

	// Initialize the interface to the Direct3D9 library
	if ( !CreateInterface() )
	{
		return false;
	}
	// Create an interface to a Direct3D device
	if ( !CreateDevice() )
	{
		goto OnError;
	}


	s_material_ceiling = new Material("data/ceiling.lua");
	s_material_floor = new Material("data/floor.lua");
	s_material_metal = new Material("data/metal.lua");
	s_material_railing = new Material("data/railing.lua");
	s_material_wall = new Material("data/wall.lua");
	s_material_cement = new Material("data/cement.lua");

	s_material_ceiling->s_direct3dDevice = s_direct3dDevice;
	s_material_floor->s_direct3dDevice = s_direct3dDevice;
	s_material_metal->s_direct3dDevice = s_direct3dDevice;
	s_material_railing->s_direct3dDevice = s_direct3dDevice;
	s_material_wall->s_direct3dDevice = s_direct3dDevice;
	s_material_cement->s_direct3dDevice = s_direct3dDevice;

	/*HRESULT result;
	result = s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	result = s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	result = s_direct3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);*/

	s_ceiling = new Mesh("data/Ceiling.mesh");
	s_floor = new Mesh("data/Floor.mesh");
	s_metal = new Mesh("data/Metal.mesh");
	s_railing = new Mesh("data/Railing.mesh");
	s_lambert2 = new Mesh("data/Lambert2.mesh");
	s_wall = new Mesh("data/Walls.mesh");
	s_cement = new Mesh("data/Cement.mesh");
	s_debugCylinder1 = new Mesh("data/Cylinder.mesh");
	

	//o_fish = new Renderable(*s_material_default, *s_fish);
	o_ceiling = new Renderable(*s_material_ceiling, *s_ceiling);
	o_floor = new Renderable(*s_material_floor, *s_floor);
	o_metal = new Renderable(*s_material_metal, *s_metal);
	o_railing = new Renderable(*s_material_railing, *s_railing);
	o_lambert2 = new Renderable(*s_material_floor, *s_lambert2);
	o_wall = new Renderable(*s_material_wall, *s_wall);
	o_cement = new Renderable(*s_material_cement, *s_cement);
	o_debugCylinder1 = new Renderable(*s_material_railing, *s_debugCylinder1);

	/*if ( !CreateIndexBuffer() )
	{
		goto OnError;
	}*/

	//s_direct3dDevice = Graphics::GetLocalDirect3dDevice();

	/*if (!o_man->LoadRenderable() || !o_floor->LoadRenderable() || !o_house->LoadRenderable() || !o_box_1->LoadRenderable() || !o_box_2->LoadRenderable() || !o_box_3->LoadRenderable() || !o_box_4->LoadRenderable())
	{
		goto OnError;
	}
	*/
	if (!o_ceiling->LoadRenderable() || !o_floor->LoadRenderable() || !o_metal->LoadRenderable() || !o_railing->LoadRenderable()
		|| !o_lambert2->LoadRenderable() || !o_wall->LoadRenderable() || !o_cement->LoadRenderable()||!o_debugCylinder1->LoadRenderable())
	{
		goto OnError;
	}



	return true;


OnError:

	ShutDown();
	return false;
}

bool eae6320::Graphics::Clear()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		const D3DRECT* subRectanglesToClear = NULL;
		const DWORD subRectangleCount = 0;
		const DWORD clearTheRenderTarget = D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER;
		D3DCOLOR clearColor;
		{
			// Black is usually used:
			clearColor = D3DCOLOR_XRGB(0, 0, 0);
		}
		const float noZBuffer = 1.0f;
		const DWORD noStencilBuffer = 0;
		HRESULT result = s_direct3dDevice->Clear(subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, noZBuffer, noStencilBuffer);
		assert(SUCCEEDED(result));
	}
	return true;
}
bool eae6320::Graphics::BeginDraw()
{
	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	
		HRESULT result = s_direct3dDevice->BeginScene();
		assert(SUCCEEDED(result));

		//// Set the shaders
		//{
		//	result = s_direct3dDevice->SetVertexShader(s_vertexShader);
		//	assert(SUCCEEDED(result));
		//	result = s_direct3dDevice->SetPixelShader(s_fragmentShader);
		//	assert(SUCCEEDED(result));
		//}

	return true;
}
			//s_rectangle_object->DrawGameObject();
			//s_leftTriangle_object->o_offset.x = -0.3f;
			//s_leftTriangle_object->DrawGameObject();
			//s_rightTriangle_object->o_offset.x = 0.3f;
			//s_rightTriangle_object->DrawGameObject();


bool eae6320::Graphics::EndDraw()
{
		HRESULT result = s_direct3dDevice->EndScene();
		assert( SUCCEEDED( result ) );

		return true;
}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be "presented"
	// (to the front buffer)
bool eae6320::Graphics::ShowBuffer()
{
	{
		const RECT* noSourceRectangle = NULL;
		const RECT* noDestinationRectangle = NULL;
		const HWND useDefaultWindow = NULL;
		const RGNDATA* noDirtyRegion = NULL;
		HRESULT result = s_direct3dDevice->Present(noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion);
		assert(SUCCEEDED(result));
	}
	return true;
	
}


bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if ( s_direct3dInterface )
	{
		if ( s_direct3dDevice )
		{
		/*	if (s_effect)
			{
				s_effect->ReleaseEffect();
				s_effect = NULL;
			}*/

		/*	if (s_man)
			{
				s_man->ReleaseMesh();
				s_man = NULL;
			}*/

			s_direct3dDevice->Release();
			s_direct3dDevice = NULL;
		}

		s_direct3dInterface->Release();
		s_direct3dInterface = NULL;
	}
	s_renderingWindow = NULL;

	return !wereThereErrors;
}
IDirect3DDevice9* eae6320::Graphics::GetLocalDirect3dDevice()
{
	return s_direct3dDevice;
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateDevice()
	{
		const UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		const D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		const DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			{
				const unsigned int useRenderingWindowDimensions = 0;
				presentationParameters.BackBufferWidth = useRenderingWindowDimensions;
				presentationParameters.BackBufferHeight = useRenderingWindowDimensions;
			}
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = s_renderingWindow;
			presentationParameters.Windowed = TRUE;
			presentationParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
			presentationParameters.EnableAutoDepthStencil = TRUE;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		HRESULT result = s_direct3dInterface->CreateDevice( useDefaultDevice, useHardwareRendering,
			s_renderingWindow, useHardwareVertexProcessing, &presentationParameters, &s_direct3dDevice );
		if ( SUCCEEDED( result ) )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "Direct3D failed to create a Direct3D9 device" );
			return false;
		}
	}

	//bool CreateIndexBuffer()
	//{
	//	// The usage tells Direct3D how this vertex buffer will be used
	//	DWORD usage = 0;
	//	{
	//		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
	//		const HRESULT result = GetVertexProcessingUsage( usage );
	//		if ( FAILED( result ) )
	//		{
	//			return false;
	//		}
	//		// Our code will only ever write to the buffer
	//		usage |= D3DUSAGE_WRITEONLY;
	//	}

	//	// Create an index buffer
	//	unsigned int bufferSize;
	//	{
	//		// We are drawing a square
	//		const unsigned int triangleCount = 2;	// How many triangles does a square have?
	//		const unsigned int vertexCountPerTriangle = 3;
	//		bufferSize = triangleCount * vertexCountPerTriangle * sizeof( uint32_t );
	//		// We'll use 32-bit indices in this class to keep things simple
	//		// (i.e. every index will be a 32 bit unsigned integer)
	//		const D3DFORMAT format = D3DFMT_INDEX32;
	//		// Place the index buffer into memory that Direct3D thinks is the most appropriate
	//		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	//		HANDLE* notUsed = NULL;
	//		const HRESULT result = s_direct3dDevice->CreateIndexBuffer( bufferSize, usage, format, useDefaultPool,
	//			&s_indexBuffer, notUsed );
	//		if ( FAILED( result ) )
	//		{
	//			eae6320::UserOutput::Print( "Direct3D failed to create an index buffer" );
	//			return false;
	//		}
	//	}
	//	// Fill the index buffer with the triangles' connectivity data
	//	{
	//		// Before the index buffer can be changed it must be "locked"
	//		uint32_t* indexData;
	//		{
	//			const unsigned int lockEntireBuffer = 0;
	//			const DWORD useDefaultLockingBehavior = 0;
	//			const HRESULT result = s_indexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
	//				reinterpret_cast<void**>( &indexData ), useDefaultLockingBehavior );
	//			if ( FAILED( result ) )
	//			{
	//				eae6320::UserOutput::Print( "Direct3D failed to lock the index buffer" );
	//				return false;
	//			}
	//		}
	//		// Fill the buffer
	//		{
	//			// EAE6320_TODO:
	//			// You will need to fill in 3 indices for each triangle that needs to be drawn.
	//			// Each index will be a 32-bit unsigned integer,
	//			// and will index into the vertex buffer array that you have created.
	//			// The order of indices is important, but the correct order will depend on
	//			// which vertex you have assigned to which spot in your vertex buffer
	//			// (also remember to maintain the correct handedness for the triangle winding order).

	//			// Triangle 0
	//			indexData[0] = 0;
	//			indexData[1] = 1;
	//			indexData[2] = 2;

	//			// Triangle 1
	//			indexData[3] = 0;
	//			indexData[4] = 2;
	//			indexData[5] = 3;
	//			// etc...
	//		}
	//		// The buffer must be "unlocked" before it can be used
	//		{
	//			const HRESULT result = s_indexBuffer->Unlock();
	//			if ( FAILED( result ) )
	//			{
	//				eae6320::UserOutput::Print( "Direct3D failed to unlock the index buffer" );
	//				return false;
	//			}
	//		}
	//	}

	//	return true;
	//}

	bool CreateInterface()
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		s_direct3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
		if ( s_direct3dInterface )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "DirectX failed to create a Direct3D9 interface" );
			return false;
		}
	}

	//bool CreateVertexBuffer()
	//{
	//	// The usage tells Direct3D how this vertex buffer will be used
	//	DWORD usage = 0;
	//	{
	//		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
	//		const HRESULT result = GetVertexProcessingUsage( usage );
	//		if ( FAILED( result ) )
	//		{
	//			return false;
	//		}
	//		// Our code will only ever write to the buffer
	//		usage |= D3DUSAGE_WRITEONLY;
	//	}

	//	// Initialize the vertex format
	//	{
	//		// These elements must match the VertexFormat::sVertex layout struct exactly.
	//		// They instruct Direct3D how to match the binary data in the vertex buffer
	//		// to the input elements in a vertex shader
	//		// (by using D3DDECLUSAGE enums here and semantics in the shader,
	//		// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
	//		// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
	//		D3DVERTEXELEMENT9 vertexElements[] =
	//		{
	//			// Stream 0

	//			// POSITION
	//			// 2 floats == 8 bytes
	//			// Offset = 0
	//			{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

	//			// COLOR0
	//			// D3DCOLOR == 4 bytes
	//			// Offset = 8
	//			{ 0, 8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

	//			// The following marker signals the end of the vertex declaration
	//			D3DDECL_END()
	//		};
	//		HRESULT result = s_direct3dDevice->CreateVertexDeclaration( vertexElements, &s_vertexDeclaration );
	//		if ( SUCCEEDED( result ) )
	//		{
	//			result = s_direct3dDevice->SetVertexDeclaration( s_vertexDeclaration );
	//			if ( FAILED( result ) )
	//			{
	//				eae6320::UserOutput::Print( "Direct3D failed to set the vertex declaration" );
	//				return false;
	//			}
	//		}
	//		else
	//		{
	//			eae6320::UserOutput::Print( "Direct3D failed to create a Direct3D9 vertex declaration" );
	//			return false;
	//		}
	//	}

	//	// Create a vertex buffer
	//	{
	//		// We are drawing one square
	//		const unsigned int vertexCount = 4;	// What is the minimum number of vertices a square needs (so that no data is duplicated)?
	//		const unsigned int bufferSize = vertexCount * sizeof( eae6320::Graphics::Mesh::sVertex );
	//		// We will define our own vertex format
	//		const DWORD useSeparateVertexDeclaration = 0;
	//		// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	//		const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	//		HANDLE* const notUsed = NULL;
	//		const HRESULT result = s_direct3dDevice->CreateVertexBuffer( bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
	//			&s_vertexBuffer, notUsed );
	//		if ( FAILED( result ) )
	//		{
	//			eae6320::UserOutput::Print( "Direct3D failed to create a vertex buffer" );
	//			return false;
	//		}
	//	}
	//	eae6320::Graphics::Mesh::sVertex *vertexData = NULL;
	//	// Fill the vertex buffer with the triangle's vertices
	//	{
	//		// Before the vertex buffer can be changed it must be "locked"
	//		
	//		{
	//			const unsigned int lockEntireBuffer = 0;
	//			const DWORD useDefaultLockingBehavior = 0;
	//			const HRESULT result = s_vertexBuffer->Lock( lockEntireBuffer, lockEntireBuffer,
	//				reinterpret_cast<void**>( &vertexData), useDefaultLockingBehavior );
	//			if ( FAILED( result ) )
	//			{
	//				eae6320::UserOutput::Print( "Direct3D failed to lock the vertex buffer" );
	//				return false;
	//			}
	//		}
	//		// Fill the buffer
	//		{
	//			// You will need to fill in two pieces of information for each vertex:
	//			//	* 2 floats for the POSITION
	//			//	* 4 uint8_ts for the COLOR

	//			// The floats for POSITION are for the X and Y coordinates, like in Assignment 02.
	//			// The difference this time is that there should be fewer (because we are sharing data).
	//			
	//			// The uint8_ts for COLOR are "RGBA", where "RGB" stands for "Red Green Blue" and "A" for "Alpha".
	//			// Conceptually each of these values is a [0,1] value, but we store them as an 8-bit value to save space
	//			// (color doesn't need as much precision as position),
	//			// which means that the data we send to the GPU will be [0,255].
	//			// For now the alpha value should _always_ be 255, and so you will choose color by changing the first three RGB values.
	//			// To make white you should use (255, 255, 255), to make black (0, 0, 0).
	//			// To make pure red you would use the max for R and nothing for G and B, so (1, 0, 0).
	//			// Experiment with other values to see what happens!

	//			//Vertex[0]
	//			vertexData[0].x = 0.0f;
	//			vertexData[0].y = 0.0f;
	//			// Red
	//			vertexData[0].r = 255;
	//			vertexData[0].g = 0;
	//			vertexData[0].b = 0;
	//			vertexData[0].a = 255;

	//			//Vertex[1]
	//			vertexData[1].x = 0.0f;
	//			vertexData[1].y = 1.0f;
	//			// Blue
	//			vertexData[1].r = 0;
	//			vertexData[1].g = 0;
	//			vertexData[1].b = 255;
	//			vertexData[1].a = 255;
	//			// etc.

	//			//Vertex[2]
	//			vertexData[2].x = 1.0f;
	//			vertexData[2].y = 1.0f;
	//			// White
	//			vertexData[2].r = 255;
	//			vertexData[2].g = 255;
	//			vertexData[2].b = 255;
	//			vertexData[2].a = 255;

	//			//Vertex[3]
	//			vertexData[3].x = 1.0f;
	//			vertexData[3].y = 0.0f;
	//			// Green
	//			vertexData[3].r = 0;
	//			vertexData[3].g = 255;
	//			vertexData[3].b = 0;
	//			vertexData[3].a = 255;
	//		}
	//		// The buffer must be "unlocked" before it can be used
	//		{
	//			const HRESULT result = s_vertexBuffer->Unlock();
	//			if ( FAILED( result ) )
	//			{
	//				eae6320::UserOutput::Print( "Direct3D failed to unlock the vertex buffer" );
	//				return false;
	//			}
	//		}
	//	}

	//	return true;
	//}

	/*HRESULT GetVertexProcessingUsage( DWORD& o_usage )
	{
		D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
		const HRESULT result = s_direct3dDevice->GetCreationParameters( &deviceCreationParameters );
		if ( SUCCEEDED( result ) )
		{
			DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
				( D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING );
			o_usage = ( vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING ) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
		}
		else
		{
			eae6320::UserOutput::Print( "Direct3D failed to get the device's creation parameters" );
		}
		return result;
	}*/

	//bool LoadFragmentShader()
	//{
	//	// Load the source code from file and compile it
	//	ID3DXBuffer* compiledShader;
	//	{
	//		const char* sourceCodeFileName = "data/fragment.shader";
	//		const D3DXMACRO defines[] =
	//		{
	//			{ "EAE6320_PLATFORM_D3D", "1" },
	//			{ NULL, NULL }
	//		};
	//		ID3DXInclude* noIncludes = NULL;
	//		const char* entryPoint = "main";
	//		const char* profile = "ps_3_0";
	//		const DWORD noFlags = 0;
	//		ID3DXBuffer* errorMessages = NULL;
	//		ID3DXConstantTable** noConstants = NULL;
	//		HRESULT result = D3DXCompileShaderFromFile( sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
	//			&compiledShader, &errorMessages, noConstants );
	//		if ( SUCCEEDED( result ) )
	//		{
	//			if ( errorMessages )
	//			{
	//				errorMessages->Release();
	//			}
	//		}
	//		else
	//		{
	//			if ( errorMessages )
	//			{
	//				std::stringstream errorMessage;
	//				errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName
	//					<< ":\n" << reinterpret_cast<char*>( errorMessages->GetBufferPointer() );
	//				eae6320::UserOutput::Print( errorMessage.str() );
	//				errorMessages->Release();
	//			}
	//			else
	//			{
	//				std::stringstream errorMessage;
	//				errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName;
	//				eae6320::UserOutput::Print( errorMessage.str() );
	//			}
	//			return false;
	//		}
	//	}
	//	// Create the fragment shader object
	//	bool wereThereErrors = false;
	//	{
	//		HRESULT result = s_direct3dDevice->CreatePixelShader( reinterpret_cast<DWORD*>( compiledShader->GetBufferPointer() ),
	//			&s_fragmentShader );
	//		if ( FAILED( result ) )
	//		{
	//			eae6320::UserOutput::Print( "Direct3D failed to create the fragment shader" );
	//			wereThereErrors = true;
	//		}
	//		compiledShader->Release();
	//	}
	//	return !wereThereErrors;
	//}

	//bool LoadVertexShader()
	//{
	//	// Load the source code from file and compile it
	//	ID3DXBuffer* compiledShader;
	//	{
	//		const char* sourceCodeFileName = "data/vertex.shader";
	//		const D3DXMACRO defines[] =
	//		{
	//			{ "EAE6320_PLATFORM_D3D", "1" },
	//			{ NULL, NULL }
	//		};
	//		ID3DXInclude* noIncludes = NULL;
	//		const char* entryPoint = "main";
	//		const char* profile = "vs_3_0";
	//		const DWORD noFlags = 0;
	//		ID3DXBuffer* errorMessages = NULL;
	//		ID3DXConstantTable** noConstants = NULL;
	//		HRESULT result = D3DXCompileShaderFromFile( sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
	//			&compiledShader, &errorMessages, noConstants );
	//		if ( SUCCEEDED( result ) )
	//		{
	//			if ( errorMessages )
	//			{
	//				errorMessages->Release();
	//			}
	//		}
	//		else
	//		{
	//			if ( errorMessages )
	//			{
	//				std::stringstream errorMessage;
	//				errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName
	//					<< ":\n" << reinterpret_cast<char*>( errorMessages->GetBufferPointer() );
	//				eae6320::UserOutput::Print( errorMessage.str() );
	//				errorMessages->Release();
	//			}
	//			else
	//			{
	//				std::stringstream errorMessage;
	//				errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName;
	//				eae6320::UserOutput::Print( errorMessage.str() );
	//			}
	//			return false;
	//		}
	//	}
	//	// Create the vertex shader object
	//	bool wereThereErrors = false;
	//	{
	//		HRESULT result = s_direct3dDevice->CreateVertexShader( reinterpret_cast<DWORD*>( compiledShader->GetBufferPointer() ),
	//			&s_vertexShader );
	//		if ( FAILED( result ) )
	//		{
	//			eae6320::UserOutput::Print( "Direct3D failed to create the vertex shader" );
	//			wereThereErrors = true;
	//		}
	//		compiledShader->Release();
	//	}
	//	return !wereThereErrors;
	//}
}
