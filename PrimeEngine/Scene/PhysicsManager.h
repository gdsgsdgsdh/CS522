#pragma once
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Utils/Array/Array.h"
#include "PhysicsComponent.h"


namespace PE {
	namespace Components {
		struct PhysicsManager : public PhysicsComponent
		{
			PE_DECLARE_CLASS(PhysicsManager);

			static void Construct(PE::GameContext& context, PE::MemoryArena arena);

			// Constructor -------------------------------------------------------------
			PhysicsManager(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself) : PhysicsComponent(context, arena, hMyself) {
				m_components.reset(512);
			}

			virtual ~PhysicsManager() {}

			// Component ------------------------------------------------------------
			void PhysicsManager::addDefaultComponents();
			

			static PhysicsManager* Instance() { return s_hInstance.getObject<PhysicsManager>(); }
			static void SetInstance(Handle h) { s_hInstance = h; }
		private:
			static Handle s_hInstance;
		};

		
	}

}
