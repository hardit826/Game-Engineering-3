#ifndef GUARD_CAMERA_H
#define GUARD_CAMERA_H

#include "../Math/cVector.h"
#include "../UserInput/UserInput.h"
#include "../Math/cQuaternion.h"
#include "../Math/Functions.h"
#include "../UserInput/UserInput.h"
#include "../Time/Time.h"
namespace eae6320
{
	namespace Graphics
	{
		struct Camera
		{
		public:
			//Cam movement Speed
		    float camSPEED = 400.0f;

			void StrafeLeft();
			void StrafeRight();
			void FlyUp();
			void DiveDown();
			void WalkForward();
			void WalkBackward();
			void RotateClockwise();
			void RotateAntiClockwise();
			void CameraControls();
			

		public:
			Math::cVector camPosition;
			Math::cQuaternion camRotation;

		public:
			Camera() :
				camPosition(Math::cVector(0, 10, 13)),
				camRotation(Math::cQuaternion())
			{};
		};
	}
}
#endif // !GUARD_CAMERA_H
