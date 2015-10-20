#ifndef EAE6320_GAMEOBJECT_H
#define EAE6320_GAMEOBJECT_H

#include "../Graphics/GraphicEffect.h"
#include "../Graphics/Mesh.h"
#include "../Math/cVector.h"

namespace eae6320
{
	namespace Graphics
	{
		class GameObject
		{
		public:
			Math::cVector o_offset;
			Graphics::Mesh o_mesh;
			Graphics::GraphicEffect o_effect;
		public:
			GameObject(Graphics::GraphicEffect i_effect,Graphics::Mesh i_mesh):
			o_effect(i_effect),
			o_mesh(i_mesh),
			o_offset(Math::cVector())
			{
				o_offset = Math::cVector(0.0f, 0.0f, 0.0f);
			};
			bool LoadGameObject();
			void DrawGameObject();
			void UpdatePosition(Math::cVector i_offset);
		};
	}
}

#endif //EAE6320_GAMEOBJECT_H