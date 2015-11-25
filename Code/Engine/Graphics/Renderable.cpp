
#include "Renderable.h"
#include "../Math/cMatrix_transformation.h"
#include "../Math/cQuaternion.h"
#include "Graphics.h"
#include "../UserInput/UserInput.h"
void eae6320::Graphics::Renderable::UpdatePosition(Math::cVector i_offset)
{
	const float SPEED = 2.0f;
	o_position = o_position + i_offset * SPEED;
}
void eae6320::Graphics::Renderable::UpdateRotation()
{
	if (eae6320::UserInput::IsKeyPressed('C'))
	{
		const float degree = Math::ConvertDegreesToRadians(60 * Time::GetSecondsElapsedThisFrame());
		Math::cVector axis(0, 1, 0);
		Math::cQuaternion quat = Math::cQuaternion(degree, axis);
		o_quaternion = o_quaternion * quat;
	}
	if (eae6320::UserInput::IsKeyPressed('X')) 
	{
		const float degree = Math::ConvertDegreesToRadians(-60 * Time::GetSecondsElapsedThisFrame());
		Math::cVector axis(0, 1, 0);
		Math::cQuaternion quat = Math::cQuaternion(degree, axis);
		o_quaternion = o_quaternion * quat;
	}

}

bool eae6320::Graphics::Renderable::LoadRenderable()
{
	if (!o_material.LoadMaterial())
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
	o_material.SetMaterial();
	o_material.SetMaterialUniform();
	o_material.SetEngineUniforms(Math::cMatrix_transformation(o_quaternion,o_position),*eae6320::Graphics::o_cam);
	o_mesh.DrawMesh();
}



