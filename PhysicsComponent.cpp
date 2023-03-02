#include "PhysicsComponent.h"

namespace PE {
	namespace Components {
		struct PhysicsComponent : public Component
		{
			PE_DECLARE_CLASS(PhysicsComponent);
			// Constructor -------------------------------------------------------------
			PhysicsComponent::PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself) : Component(context, arena, hMyself), m_lights(context, arena, 8), m_inheritPositionOnly(false)
			{
			}

			virtual ~PhysicsComponent() {}

			// Component ------------------------------------------------------------

			virtual void addDefaultComponents();

			// Individual events -------------------------------------------------------
		};


	}
