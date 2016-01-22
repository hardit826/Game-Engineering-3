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
	o_ceiling->DrawRenderable();
	o_floor->DrawRenderable();
	o_metal->DrawRenderable();
	o_railing->DrawRenderable();
	o_lambert2->DrawRenderable();
	o_wall->DrawRenderable();
	o_cement->DrawRenderable();
}