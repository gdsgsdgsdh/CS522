#pragma once

#include <vector>
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"


// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Events/StandardEvents.h"
#include "../Utils/Array/Array.h"
#include "Mesh.h"
#include "MeshInstance.h"
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "SceneNode.h"


namespace PE {
	namespace Components {
		struct PhysicsComponent : public Component
		{
			PE_DECLARE_CLASS(PhysicsComponent);
			// Constructor -------------------------------------------------------------
			PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself);

			virtual ~PhysicsComponent() {}

			// Component ------------------------------------------------------------
			virtual void addDefaultComponents();
			void createAABB();

			// Individual events -------------------------------------------------------
			void setVerticesForAABB(std::vector<std::pair<Vector3, Vector3>> sides);
		private:
			std::vector<std::pair<Vector3, Vector3>> sides;
		};
	}
}
