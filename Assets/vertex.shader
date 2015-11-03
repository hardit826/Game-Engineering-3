/*
	This is an example of a vertex shader
*/
#include "shaders.inc"

//uniform declaration for D3D
uniform float2 g_position_offset;

#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateVertexDeclaration()

	// These values come from one of the sVertex that we filled the vertex buffer with in C code
	in const float2 i_position : POSITION,
	in const float4 i_color : COLOR,

	// Output
	//=======

	// A POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 o_position : POSITION,

	// Any other data is optional; the GPU doesn't know or care what it is,
	// and will merely interpolate it across the triangle
	// and give us the resulting interpolated value in a fragment shader.
	// It is then up to us to use it however we want to.
	// The "semantics" are used to match the vertex shader outputs
	// with the fragment shader inputs
	// (note that OpenGL uses arbitrarily assignable number IDs to do the same thing).
	out float4 o_color : COLOR

	)

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======


// These values come from one of the sVertex that we filled the vertex buffer with in C code
layout( location = 0 ) in vec2 i_position;
layout( location = 1 ) in vec4 i_color;

// Output
//=======

// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an implicit required variable called "gl_Position"

// Any other data is optional; the GPU doesn't know or care what it is,
// and will merely interpolate it across the triangle
// and give us the resulting interpolated value in a fragment shader.
// It is then up to us to use it however we want to.
// The locations are used to match the vertex shader outputs
// with the fragment shader inputs
// (note that Direct3D uses arbitrarily assignable "semantics").
layout( location = 0 ) out vec4 o_color;

// Entry Point
//============

void main()

#endif

{
	// Calculate the position of this vertex on screen
	{
		// When we move to 3D graphics the screen position that the vertex shader outputs
		// will be different than the position that is input to it from C code,
		// but for now the "out" position is set directly from the "in" position:
		//o_position = float4( i_position.x, i_position.y, 0.0, 1.0 );
		
#if defined( EAE6320_PLATFORM_D3D )
		o_position 
#elif defined( EAE6320_PLATFORM_GL )
		gl_Position
#endif
    	= vec4(i_position + g_position_offset, 0.0, 1.0);
		// Or, equivalently:
		// o_position = float4( i_position.xy, 0.0, 1.0 );
		// o_position = float4( i_position, 0.0, 1.0 );
	}
	// Pass the input color to the fragment shader unchanged:
	{
		o_color = i_color;
	}
}