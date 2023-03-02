#pragma once
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


namespace PE {
	namespace Components {
		struct PhysicsComponent : public Component
		{
			PE_DECLARE_CLASS(PhysicsComponent);
			// Constructor -------------------------------------------------------------
			PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself);

			virtual ~PhysicsComponent() {}

			// Component ------------------------------------------------------------

			static void createAABB(MeshInstance* pInst, Mesh* pMeshCaller);

			virtual void addDefaultComponents();

			// Individual events -------------------------------------------------------
		};

		struct AABBHelpers
		{
			static void generateVertexForAABB(const Vector3& min, const Vector3& max, Vector3 vertices[8]);
		};
		
	}

}
