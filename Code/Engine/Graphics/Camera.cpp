#include "Camera.h"
#include "../Math/cVector.h"

void eae6320::Graphics::Camera::StrafeLeft()
{
	if (eae6320::UserInput::IsKeyPressed('A'))
	{
		camPosition.x -= camSPEED* eae6320::Time::GetSecondsElapsedThisFrame();
	}
}
void eae6320::Graphics::Camera::StrafeRight()
{
	if (eae6320::UserInput::IsKeyPressed('D'))
	{
		camPosition.x += camSPEED* eae6320::Time::GetSecondsElapsedThisFrame();
	}
}
void eae6320::Graphics::Camera::FlyUp()
{
	if (eae6320::UserInput::IsKeyPressed(' '))
	{
		camPosition.y += camSPEED* eae6320::Time::GetSecondsElapsedThisFrame();
	}
}
void eae6320::Graphics::Camera::DiveDown()
{
	if (eae6320::UserInput::IsKeyPressed('Z'))
	{
		camPosition.y -= camSPEED* eae6320::Time::GetSecondsElapsedThisFrame();
	}
}

void eae6320::Graphics::Camera::WalkForward()
{
	if (eae6320::UserInput::IsKeyPressed('W'))
	{
		camPosition.z -= camSPEED* eae6320::Time::GetSecondsElapsedThisFrame();
	}
}
void eae6320::Graphics::Camera::WalkBackward()
{
	if (eae6320::UserInput::IsKeyPressed('S'))
	{
		camPosition.z += camSPEED* eae6320::Time::GetSecondsElapsedThisFrame();
	}
}
void eae6320::Graphics::Camera::RotateClockwise()
{
	if (eae6320::UserInput::IsKeyPressed('E'))
	{
		const float degree = Math::ConvertDegreesToRadians(60 * Time::GetSecondsElapsedThisFrame());
		Math::cVector axis(0, 1, 0);
		Math::cQuaternion quat = Math::cQuaternion(degree, axis);
		camRotation = camRotation * quat;
	}
}
void eae6320::Graphics::Camera::RotateAntiClockwise()
{
	if (eae6320::UserInput::IsKeyPressed('Q'))
	{
		const float degree = Math::ConvertDegreesToRadians(-60 * Time::GetSecondsElapsedThisFrame());
		Math::cVector axis(0, 1, 0);
		Math::cQuaternion quat = Math::cQuaternion(degree, axis);
		camRotation = camRotation * quat;
	}
}
void eae6320::Graphics::Camera::CameraControls()
{
	StrafeLeft();
	StrafeRight();
	FlyUp();
	DiveDown();
	WalkForward();
	WalkBackward();
	RotateClockwise();
	RotateAntiClockwise();
}