
#include "Renderable.h"
#include "../Math/cMatrix_transformation.h"

void eae6320::Graphics::Renderable::UpdatePosition(Math::cVector i_offset)
{
	o_position = o_position + i_offset;
}

bool eae6320::Graphics::Renderable::LoadRenderable()
{
	if (!o_effect.LoadShaders())
	{
		return false;
	}
	if (!o_mesh.LoadMesh())
	{
		return false;
	}
	return true;
}

void eae6320::Graphics::Renderable::DrawRenderable()
{
	o_effect.SetPath();
	o_effect.SetDrawCallUniforms(Math::cMatrix_transformation(o_quaternion,o_position));
	o_mesh.DrawMesh();
}



