#include "Renderable.h"
#include "Graphics.h"

void eae6320::Graphics::Render()
{
	Clear();
	BeginDraw();
	DrawRenderableList();
	EndDraw();
	ShowBuffer();
}

void eae6320::Graphics::DrawRenderableList()
{
	//man mesh
	o_man->DrawRenderable();
	//floor mesh
	o_floor->DrawRenderable();
	//house mesh
	o_house->DrawRenderable();
	//sphere mesh
	o_sphere->DrawRenderable();
}