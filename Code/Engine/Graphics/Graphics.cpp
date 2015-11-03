#include "GameObject.h"
#include "Graphics.h"

void eae6320::Graphics::Render()
{
	Clear();
	BeginDraw();
	s_rectangle_object->DrawGameObject();
	s_leftTriangle_object->o_offset.x = -0.3f;
	s_leftTriangle_object->DrawGameObject();
	s_rightTriangle_object->o_offset.x = 0.3f;
	s_rightTriangle_object->DrawGameObject();
	EndDraw();
	ShowBuffer();
}