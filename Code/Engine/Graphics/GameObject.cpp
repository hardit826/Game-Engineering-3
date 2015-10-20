
#include "GameObject.h"

void eae6320::Graphics::GameObject::UpdatePosition(Math::cVector i_offset)
{
	o_offset = o_offset + i_offset;
}

bool eae6320::Graphics::GameObject::LoadGameObject()
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

void eae6320::Graphics::GameObject::DrawGameObject()
{
	o_effect.SetPath();
	o_effect.SetDrawCallUniforms(o_offset);
	o_mesh.DrawMesh();
}



