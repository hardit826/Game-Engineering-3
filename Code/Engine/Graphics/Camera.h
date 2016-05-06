#ifndef GUARD_CAMERA_H
#define GUARD_CAMERA_H

#include "../Math/cVector.h"
#include "../UserInput/UserInput.h"
#include "../Math/cQuaternion.h"
#include "../Math/Functions.h"
#include "../UserInput/UserInput.h"
#include "../Time/Time.h"
#include "../Math/cQuaternion.h"

namespace eae6320
{
	namespace Graphics
	{
		struct Camera
		{
		public:
			Math::cVector m_position;
			Math::cQuaternion m_orientation;
			float m_fieldOfView_y;
			float m_aspectRatio;
			float m_z_nearPlane;
			float m_z_farPlane;

		public:
			Camera() :
				m_position(Math::cVector(0, 0, 100)),
				m_orientation(Math::cQuaternion()),
				m_fieldOfView_y(Math::ConvertDegreesToRadians(60)),
				m_aspectRatio((float)800 / 600),
				m_z_nearPlane(0.1f),
				m_z_farPlane(10000)
			{
			};

			void UpdatePosition(Math::cVector i_position_offset);
			void UpdateOrientation(Math::cVector i_rotation_offset);
		};

	}
}
#endif // !GUARD_CAMERA_H
