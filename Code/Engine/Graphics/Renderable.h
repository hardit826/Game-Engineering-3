#ifndef EAE6320_GAMEOBJECT_H
#define EAE6320_GAMEOBJECT_H

#include "../Graphics/GraphicEffect.h"
#include "../Graphics/Mesh.h"
#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"
#include "Material.h"

namespace eae6320
{
	namespace Graphics
	{
		class Renderable
		{
		public:
			
			Math::cVector o_position;
			Math::cQuaternion o_quaternion;
			Graphics::Mesh o_mesh;
			//Graphics::GraphicEffect o_effect;
			Material o_material;

		public:
			Renderable(Material i_material,Graphics::Mesh i_mesh):
			o_material(i_material),
			o_mesh(i_mesh),
			o_position(Math::cVector()),
			o_quaternion(Math::cQuaternion()){}

			bool LoadRenderable();
			void DrawRenderable();
			void UpdatePosition(Math::cVector i_offset);
			void UpdateRotation();
		};
	}
}

#endif //EAE6320_GAMEOBJECT_H