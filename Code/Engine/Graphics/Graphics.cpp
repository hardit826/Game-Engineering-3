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
	//cube mesh
	o_cube->DrawRenderable();
}