// Header Files
//=============

#include "../Graphics.h"
#include "../GraphicEffect.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <sstream>
#include "../../UserOutput/UserOutput.h"
#include "../../Windows/WindowsFunctions.h"
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../../Graphics/Mesh.h"
#include "../Renderable.h"

// Static Data Initialization
//===========================

eae6320::Graphics::Renderable* eae6320::Graphics::o_ceiling = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_floor = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_metal = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_railing = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_lambert2 = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_wall = NULL;
eae6320::Graphics::Renderable* eae6320::Graphics::o_cement = NULL;

eae6320::Graphics::Camera* eae6320::Graphics::o_cam = NULL;

namespace
{

	HWND s_renderingWindow = NULL;
	HDC s_deviceContext = NULL;
	HGLRC s_openGlRenderingContext = NULL;
	//eae6320::Graphics::GraphicEffect* s_effect;
	//eae6320::Graphics::GraphicEffect* s_effect_transparent;
	eae6320::Graphics::Material * s_material_ceiling;
	eae6320::Graphics::Material * s_material_floor;
	eae6320::Graphics::Material * s_material_metal;
	eae6320::Graphics::Material * s_material_railing;
	eae6320::Graphics::Material * s_material_wall;
	eae6320::Graphics::Material * s_material_cement;


	eae6320::Graphics::Mesh *s_ceiling = NULL;
	eae6320::Graphics::Mesh *s_floor = NULL;
	eae6320::Graphics::Mesh *s_metal = NULL;
	eae6320::Graphics::Mesh *s_railing = NULL;
	eae6320::Graphics::Mesh *s_lambert2 = NULL;
	eae6320::Graphics::Mesh *s_wall = NULL;
	eae6320::Graphics::Mesh *s_cement = NULL;

	//// This struct determines the layout of the data that the CPU will send to the GPU
	//struct sVertex
	//{
	//	// POSITION
	//	// 2 floats == 8 bytes
	//	// Offset = 0
	//	float x, y;
	//	// COLOR0
	//	// 4 uint8_ts == 4 bytes
	//	// Offset = 8
	//	uint8_t r, g, b, a;	// 8 bits [0,255] per RGBA channel (the alpha channel is unused but is present so that color uses a full 4 bytes)
	//};

	// A vertex array encapsulates both the vertex and index data as well as the vertex format
	GLuint s_vertexArrayId = 0;

	// OpenGL encapsulates a matching vertex shader and fragment shader into what it calls a "program".

	// A vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want

	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	GLuint s_programId = 0;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateRenderingContext();
	//bool CreateVertexArray();
	//bool LoadAndAllocateShaderProgram( const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage );
	//bool LoadFragmentShader( const GLuint i_programId );
	//bool LoadVertexShader( const GLuint i_programId );

	// This helper struct exists to be able to dynamically allocate memory to get "log info"
	// which will automatically be freed when the struct goes out of scope
	struct sLogInfo
	{
		GLchar* memory;
		sLogInfo( const size_t i_size ) { memory = reinterpret_cast<GLchar*>( malloc( i_size ) ); }
		~sLogInfo() { if ( memory ) free( memory ); }
	};
}

// Interface
//==========

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	s_renderingWindow = i_renderingWindow;

	//s_effect = new GraphicEffect("data/effect.lua");
	//s_effect_transparent = new GraphicEffect("data/effect_transparent.lua");


	s_material_ceiling = new Material("data/ceiling.lua");
	s_material_floor = new Material("data/floor.lua");
	s_material_metal = new Material("data/metal.lua");
	s_material_railing = new Material("data/railing.lua");
	s_material_wall = new Material("data/wall.lua");
	s_material_cement = new Material("data/cement.lua");

	s_ceiling = new Mesh("data/Ceiling.mesh");
	s_floor = new Mesh("data/Floor.mesh");
	s_metal = new Mesh("data/Metal.mesh");
	s_railing = new Mesh("data/Railing.mesh");
	s_lambert2 = new Mesh("data/Lambert2.mesh");
	s_wall = new Mesh("data/Walls.mesh");
	s_cement = new Mesh("data/Cement.mesh");
	
	o_cam = new Camera();

	// Create an OpenGL rendering context
	if ( !CreateRenderingContext() )
	{
		goto OnError;
	}

	// Load any required OpenGL extensions
	{
		std::string errorMessage;
		if ( !OpenGlExtensions::Load( &errorMessage ) )
		{
			UserOutput::Print( errorMessage );
			goto OnError;
		}
	}
 
	assert(glGetError() == GL_NO_ERROR);

	// Initialize the graphics objects
	/*if ( !s_Mesh_Rectangle->LoadMesh("data/rectangle.mesh")|| !s_Mesh_Triangle->LoadMesh("data/triangle.mesh"))
	{
		goto OnError;
	}*/

	o_ceiling = new Renderable(*s_material_ceiling, *s_ceiling);
	o_floor = new Renderable(*s_material_floor, *s_floor);
	o_metal = new Renderable(*s_material_metal, *s_metal);
	o_railing = new Renderable(*s_material_railing, *s_railing);
	o_lambert2 = new Renderable(*s_material_floor, *s_lambert2);
	o_wall = new Renderable(*s_material_wall, *s_wall);
	o_cement = new Renderable(*s_material_cement, *s_cement);

	/*if (!o_man->LoadRenderable() || !o_floor->LoadRenderable() || !o_house->LoadRenderable() || !o_box_1->LoadRenderable() || !o_box_2->LoadRenderable() || !o_box_3->LoadRenderable() || !o_box_4->LoadRenderable())
	{
		goto OnError;
	}*/

	if (!o_ceiling->LoadRenderable() || !o_floor->LoadRenderable() || !o_metal->LoadRenderable() || !o_railing->LoadRenderable()
		|| !o_lambert2->LoadRenderable() || !o_wall->LoadRenderable() || !o_cement->LoadRenderable())
	{
		goto OnError;
	}

	assert(glGetError() == GL_NO_ERROR);
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
		// Black is usually used
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		int errorcode = glGetError();
		assert(glGetError() == GL_NO_ERROR);
		// In addition to the color, "depth" and "stencil" can also be cleared,
		// but for now we only care about color
		glDepthMask(GL_TRUE);

		const GLbitfield clearColor = GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT;
		glClear(clearColor);
		assert(glGetError() == GL_NO_ERROR);
	}
	return true;
}
bool eae6320::Graphics::BeginDraw()
{
	return true;
}
bool eae6320::Graphics::EndDraw()
{
	return true;
}
bool eae6320::Graphics::ShowBuffer()
{
	// The actual function calls that draw geometry
	{
		// Set the vertex and fragment shaders

		/*{
			glUseProgram( s_programId );
			assert( glGetError() == GL_NO_ERROR );
		}*/

		/*s_rectangle_object->DrawGameObject();
		s_leftTriangle_object->o_offset.x = -0.3f;
		s_leftTriangle_object->DrawGameObject();
		s_rightTriangle_object->o_offset.x = 0.3f;
		s_rightTriangle_object->DrawGameObject();*/

		//// Bind a specific vertex buffer to the device as a data source
		//{
		//	glBindVertexArray( s_vertexArrayId );
		//	assert( glGetError() == GL_NO_ERROR );
		//}
		//// Render objects from the current streams
		//{
		//	// We are using triangles as the "primitive" type,
		//	// and we have defined the vertex buffer as a triangle list
		//	// (meaning that every triangle is defined by three vertices)
		//	const GLenum mode = GL_TRIANGLES;
		//	// We'll use 32-bit indices in this class to keep things simple
		//	// (i.e. every index will be a 32 bit unsigned integer)
		//	const GLenum indexType = GL_UNSIGNED_INT;
		//	// It is possible to start rendering in the middle of an index buffer
		//	const GLvoid* const offset = 0;
		//	// We are drawing a square
		//	const GLsizei primitiveCountToRender = 2;	// How many triangles will be drawn?
		//	const GLsizei vertexCountPerTriangle = 3;
		//	const GLsizei vertexCountToRender = primitiveCountToRender * vertexCountPerTriangle;
		//	glDrawElements( mode, vertexCountToRender, indexType, offset );
		//	assert( glGetError() == GL_NO_ERROR );
		//}
	}

	{
		BOOL result = SwapBuffers( s_deviceContext );
		assert( result != FALSE );
	}
	return true;
}

bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if ( s_openGlRenderingContext != NULL )
	{
		if ( s_programId != 0 )
		{
			glDeleteProgram( s_programId );
			const GLenum errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to delete the program: " <<
					reinterpret_cast<const char*>( gluErrorString( errorCode ) );
				UserOutput::Print( errorMessage.str() );
			}
			s_programId = 0;
		}
		if ( s_vertexArrayId != 0 )
		{
			const GLsizei arrayCount = 1;
			glDeleteVertexArrays( arrayCount, &s_vertexArrayId );
			const GLenum errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to delete the vertex array: " <<
					reinterpret_cast<const char*>( gluErrorString( errorCode ) );
				UserOutput::Print( errorMessage.str() );
			}
			s_vertexArrayId = 0;
		}

		if ( wglMakeCurrent( s_deviceContext, NULL ) != FALSE )
		{
			if ( wglDeleteContext( s_openGlRenderingContext ) == FALSE )
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to delete the OpenGL rendering context: " << GetLastWindowsError();
				UserOutput::Print( errorMessage.str() );
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "Windows failed to unset the current OpenGL rendering context: " << GetLastWindowsError();
			UserOutput::Print( errorMessage.str() );
		}
		s_openGlRenderingContext = NULL;
	}

	if ( s_deviceContext != NULL )
	{
		// The documentation says that this call isn't necessary when CS_OWNDC is used
		ReleaseDC( s_renderingWindow, s_deviceContext );
		s_deviceContext = NULL;
	}

	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Declarations
//=============================

namespace
{

	bool CreateRenderingContext()
	{
		// A "device context" can be thought of an abstraction that Windows uses
		// to represent the graphics adaptor used to display a given window
		s_deviceContext = GetDC( s_renderingWindow );
		if ( s_deviceContext == NULL )
		{
			eae6320::UserOutput::Print( "Windows failed to get the device context" );
			return false;
		}
		// Windows requires that an OpenGL "render context" is made for the window we want to use to render into
		{
			// Set the pixel format of the rendering window
			{
				PIXELFORMATDESCRIPTOR desiredPixelFormat = { 0 };
				{
					desiredPixelFormat.nSize = sizeof( PIXELFORMATDESCRIPTOR );
					desiredPixelFormat.nVersion = 1;

					desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
					desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
					desiredPixelFormat.cColorBits = 32;
					desiredPixelFormat.iLayerType = PFD_MAIN_PLANE ;
					desiredPixelFormat.cDepthBits = 16;
				}
				// Get the ID of the desired pixel format
				int pixelFormatId;
				{
					pixelFormatId = ChoosePixelFormat( s_deviceContext, &desiredPixelFormat );
					if ( pixelFormatId == 0 )
					{
						std::stringstream errorMessage;
						errorMessage << "Windows couldn't choose the closest pixel format: " << eae6320::GetLastWindowsError();
						eae6320::UserOutput::Print( errorMessage.str() );
						return false;
					}
				}
				// Set it
				if ( SetPixelFormat( s_deviceContext, pixelFormatId, &desiredPixelFormat ) == FALSE )
				{
					std::stringstream errorMessage;
					errorMessage << "Windows couldn't set the desired pixel format: " << eae6320::GetLastWindowsError();
					eae6320::UserOutput::Print( errorMessage.str() );
					return false;
				}
			}
			// Create the OpenGL rendering context
			s_openGlRenderingContext = wglCreateContext( s_deviceContext );
			if ( s_openGlRenderingContext == NULL )
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to create an OpenGL rendering context: " << eae6320::GetLastWindowsError();
				eae6320::UserOutput::Print( errorMessage.str() );
				return false;
			}
			// Set it as the rendering context of this thread
			if ( wglMakeCurrent( s_deviceContext, s_openGlRenderingContext ) == FALSE )
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to set the current OpenGL rendering context: " << eae6320::GetLastWindowsError();
				eae6320::UserOutput::Print( errorMessage.str() );
				return false;
			}
		}
		

		return true;
	}
}
