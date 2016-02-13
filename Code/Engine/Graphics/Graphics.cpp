#include "Renderable.h"
#include "Graphics.h"
#include "DebugShapes.h"


eae6320::Graphics::DebugLine s_debugLine1;
eae6320::Graphics::DebugBox s_debugBox1;
eae6320::Graphics::DebugSphere s_debugSphere1;


eae6320::Graphics::DebugLine s_debugLine2;
eae6320::Graphics::DebugBox s_debugBox2;
eae6320::Graphics::DebugSphere s_debugSphere2;

void eae6320::Graphics::Render()
{
	Clear();
	BeginDraw();
	DrawRenderableList();

	//load draw objects
	LoadObjects();
	DrawObjects();

	EndDraw();
	ShowBuffer();
}

void eae6320::Graphics::DrawRenderableList()
{
	s_debugLine1 = eae6320::Graphics::DebugLine(Math::cVector(100.0f, 0.0f, 0.0f), Math::cVector(75.0f, -50.0f, -50.0f), Math::cVector(0.0f, 0.0f, 1.0f));
	s_debugBox1 = eae6320::Graphics::DebugBox(Math::cVector(30.0f, -20.0f, -40.0f), 20.0f, Math::cVector(0.0f, 0.0f, 1.0f));
	s_debugSphere1 = eae6320::Graphics::DebugSphere(Math::cVector(-50.0f, 0.0f, -40.0f), 30.0f, 20, 20, Math::cVector(0.0f, 0.0f, 1.0f));

	s_debugLine2 = eae6320::Graphics::DebugLine(Math::cVector(100.0f, -50.0f, 0.0f), Math::cVector(75.0f, -100.0f, -50.0f), Math::cVector(1.0f, 1.0f, 0.0f));
	s_debugBox2 = eae6320::Graphics::DebugBox(Math::cVector(30.0f, -100.0f, -40.0f), 20.0f, Math::cVector(1.0f, 1.0f, 0.0f));
	s_debugSphere2 = eae6320::Graphics::DebugSphere(Math::cVector(-50.0f, -80.0f, -40.0f), 30.0f, 20, 20, Math::cVector(1.0f, 1.0f, 0.0f));

	o_ceiling->DrawRenderable();
	o_floor->DrawRenderable();
	o_metal->DrawRenderable();
	o_railing->DrawRenderable();
	o_lambert2->DrawRenderable();
	o_wall->DrawRenderable();
	o_cement->DrawRenderable();

#ifdef _DEBUG
	Graphics::GetLocalDirect3dDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	o_debugCylinder1->DrawRenderable();
	Graphics::GetLocalDirect3dDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}
bool eae6320::Graphics::LoadObjects()
{
	s_debugLine1.LoadDebugLine();
	s_debugBox1.LoadDebugBox();
	s_debugSphere1.LoadDebugSphere();

	s_debugLine2.LoadDebugLine();
	s_debugBox2.LoadDebugBox();
	s_debugSphere2.LoadDebugSphere();

	return true;
}
bool eae6320::Graphics::DrawObjects()
{
#ifdef _DEBUG
	s_debugLine1.DrawLine();
	s_debugBox1.DrawBox();
	s_debugSphere1.DrawSphere();

	s_debugLine2.DrawLine();
	s_debugBox2.DrawBox();
	s_debugSphere2.DrawSphere();
#endif
	return true;
}