#include "Renderable.h"
#include "Graphics.h"
#include "DebugShapes.h"


eae6320::Graphics::DebugLine s_debugLine1;
eae6320::Graphics::DebugBox s_debugBox1;
eae6320::Graphics::DebugSphere s_debugSphere1;

eae6320::Graphics::DebugLine s_debugLine2;
eae6320::Graphics::DebugBox s_debugBox2;
eae6320::Graphics::DebugSphere s_debugSphere2;
eae6320::Graphics::GameSprite s_logo;

eae6320::Graphics::DebugMenuSelection eae6320::Graphics::s_activeMenuItem;
eae6320::Graphics::DebugMenuText s_debugMenuTextFPS;
eae6320::Graphics::DebugMenuCheckBox* eae6320::Graphics::s_debugMenuCheckBox = NULL;
eae6320::Graphics::DebugMenuSlider* eae6320::Graphics::s_debugMenuSlider = NULL;
eae6320::Graphics::DebugMenuButton* eae6320::Graphics::s_debugMenuButton = NULL;
eae6320::Graphics::DebugMenuCheckBox* eae6320::Graphics::s_toggleFPSCheckBox = NULL;

bool eae6320::Graphics::s_debugMenuEnabled = false;

void eae6320::Graphics::Render()
{
	Clear();
	BeginDraw();
	DrawRenderableList();

	//load draw objects
	//LoadObjects();
	DrawObjects();

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




#ifdef _DEBUG
	Graphics::GetLocalDirect3dDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	o_debugCylinder1->DrawRenderable();
	Graphics::GetLocalDirect3dDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

bool eae6320::Graphics::LoadObjects()
{
	s_debugLine1 = eae6320::Graphics::DebugLine(Math::cVector(100.0f, 0.0f, 0.0f), Math::cVector(75.0f, -50.0f, -50.0f), Math::cVector(0.0f, 0.0f, 1.0f));
	s_debugBox1 = eae6320::Graphics::DebugBox(Math::cVector(30.0f, -20.0f, -40.0f), 20.0f, Math::cVector(0.0f, 0.0f, 1.0f));
	s_debugSphere1 = new eae6320::Graphics::DebugSphere(Math::cVector(-50.0f, 0.0f, -40.0f), 30.0f, 20, 20, Math::cVector(0.0f, 0.0f, 1.0f));

	s_debugLine2 = eae6320::Graphics::DebugLine(Math::cVector(100.0f, -50.0f, 0.0f), Math::cVector(75.0f, -100.0f, -50.0f), Math::cVector(1.0f, 1.0f, 0.0f));
	s_debugBox2 = eae6320::Graphics::DebugBox(Math::cVector(30.0f, -100.0f, -40.0f), 20.0f, Math::cVector(1.0f, 1.0f, 0.0f));
	s_debugSphere2 = eae6320::Graphics::DebugSphere(Math::cVector(-50.0f, -80.0f, -40.0f), 30.0f, 20, 20, Math::cVector(1.0f, 1.0f, 0.0f));

	s_debugLine1.LoadDebugLine();
	s_debugBox1.LoadDebugBox();
	s_debugSphere1->LoadDebugSphere();

	s_debugLine2.LoadDebugLine();
	s_debugBox2.LoadDebugBox();
	s_debugSphere2.LoadDebugSphere();
	s_logo = GameSprite(10, 10);
	s_logo.Initialize(GetLocalDirect3dDevice(), "data/logo.png", 256, 256);

#ifdef _DEBUG
	// Debug Menu Stuff
	s_debugMenuTextFPS = eae6320::Graphics::DebugMenuText("FPS = ", 20, 20, 150, 50);
	s_debugMenuCheckBox = new eae6320::Graphics::DebugMenuCheckBox("Debug Sphere Enabled", 20, 50, 200, 50);
	s_debugMenuSlider = new eae6320::Graphics::DebugMenuSlider("Debug Sphere Radius", 20, 80, 200, 50);
	s_debugMenuButton = new eae6320::Graphics::DebugMenuButton("Debug Sphere Radius Reset", 20, 110, 275, 50);
	//s_toggleFPSCheckBox = new eae6320::Graphics::DebugMenuCheckBox("Enable Fly Camera ", 20, 140, 200, 50);
#endif

#ifdef _DEBUG
	// Loading DebugMebu Items
	s_activeMenuItem = DebugMenuSelection::Text;
	s_debugMenuTextFPS.LoadDebugText();
	s_debugMenuCheckBox->LoadDebugCheckBox();
	s_debugMenuSlider->LoadDebugSlider();
	s_debugMenuButton->LoadDebugButton();
	//s_toggleFPSCheckBox->LoadDebugCheckBox();
#endif

	return true;
}


bool eae6320::Graphics::DrawObjects()
{
	s_logo.Draw();
	s_numbers->Draw();

#ifdef _DEBUG
	s_debugLine1.DrawLine();
	s_debugBox1.DrawBox();

	if(s_debugMenuCheckBox->m_isChecked)
		s_debugSphere1->DrawSphere();

	s_debugLine2.DrawLine();
	s_debugBox2.DrawBox();
	s_debugSphere2.DrawSphere();

	if (s_debugMenuEnabled)
	{
		float fpsCount = 1 / Time::GetSecondsElapsedThisFrame();
		s_debugMenuTextFPS.SetFPS(fpsCount);

		switch (s_activeMenuItem)
		{
		case DebugMenuSelection::Text:
			s_debugMenuTextFPS.DrawDebugText(255);
			s_debugMenuCheckBox->DrawDebugCheckBox(0);
			s_debugMenuSlider->DrawDebugSlider(0);
			s_debugMenuButton->DrawDebugButton(0);
			//s_toggleFPSCheckBox->DrawDebugCheckBox(0);
			break;

		case DebugMenuSelection::CheckBox:
			s_debugMenuTextFPS.DrawDebugText(0);
			s_debugMenuCheckBox->DrawDebugCheckBox(255);
			s_debugMenuSlider->DrawDebugSlider(0);
			s_debugMenuButton->DrawDebugButton(0);
			//s_toggleFPSCheckBox->DrawDebugCheckBox(0);
			break;

		case DebugMenuSelection::Slider:
			s_debugMenuTextFPS.DrawDebugText(0);
			s_debugMenuCheckBox->DrawDebugCheckBox(0);
			s_debugMenuSlider->DrawDebugSlider(255);
			s_debugMenuButton->DrawDebugButton(0);
			//s_toggleFPSCheckBox->DrawDebugCheckBox(0);
			break;

		case DebugMenuSelection::Button:
			s_debugMenuTextFPS.DrawDebugText(0);
			s_debugMenuCheckBox->DrawDebugCheckBox(0);
			s_debugMenuSlider->DrawDebugSlider(0);
			s_debugMenuButton->DrawDebugButton(255);
			//s_toggleFPSCheckBox->DrawDebugCheckBox(0);
			break;

		/*case DebugMenuSelection::ToggleCam:
			s_debugMenuTextFPS.DrawDebugText(0);
			s_debugMenuCheckBox->DrawDebugCheckBox(0);
			s_debugMenuSlider->DrawDebugSlider(0);
			s_debugMenuButton->DrawDebugButton(0);
			s_toggleFPSCheckBox->DrawDebugCheckBox(255);
			break;*/

		default:
			break;
		}



	}
#endif

	return true;
}