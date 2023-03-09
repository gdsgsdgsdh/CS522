#pragma once
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"
#include "unordered_set"
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

		struct PhysicsManager : public Component
		{
			struct RectangularPrism {
				RectangularPrism() {};
				RectangularPrism(std::vector<Vector3> vertices, std::vector<Vector3> normals) : vertices(vertices), normals(normals) {};
				Matrix4x4 m_base;
				std::vector<Vector3> vertices;  // 8 vertices of the rectangular prism
				std::vector<Vector3> normals;   // 6 face normals of the rectangular prism
				std::vector<std::vector<int>> faces = { {0,1,5,4}, {1,3,7,5}, {4,5,7,6}, {0,2,6,4}, {0,1,3,2}, {2,3,7,6} };
			};
			PE_DECLARE_CLASS(PhysicsManager);

			static void Construct(PE::GameContext& context, PE::MemoryArena arena);

			// Constructor -------------------------------------------------------------
			PhysicsManager(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself) : Component(context, arena, hMyself) {
				m_components.reset(512);
			}

			virtual ~PhysicsManager() {}

			// Component ------------------------------------------------------------
			void PhysicsManager::addDefaultComponents();
			

			static PhysicsManager* Instance() { return s_hInstance.getObject<PhysicsManager>(); }
			static void SetInstance(Handle h) { s_hInstance = h; }
			std::vector<Vector3> PhysicsManager::calculate_normals(std::vector<Vector3> vertices);
			std::pair<float, float> project_onto_axis(const RectangularPrism& rp, Vector3 axis);
			bool PhysicsManager::Intersect(RectangularPrism RpA, RectangularPrism RpB);
			int distanceToPointToLevel(Vector3 point, RectangularPrism rP);
			void addObjectPosition(std::vector<Vector3> vertices, Matrix4x4 m_base);	
			static void PhysicsManager::updateVertices(std::vector<Vector3>& vertices, const Vector3& dir, const float& dist);
			static void PhysicsManager::updatePos(Vector3& pos, const Vector3& dir, const float& dist);
			void updatePosition(RectangularPrism& curInst, const Vector3& dir);
			bool checkIfCollison(std::vector<Vector3> vertices, Vector3 m_base, Vector3& dir, float dist);
			void UpdateDir(const RectangularPrism& RpA, const RectangularPrism& RpB, Vector3& dir);
			void addPhysicsComponent(Handle h) { StaticPhysicsComponents.emplace_back(h); }
			bool checkPhyscisComponents(Handle h);
			int getSize() { return StaticPhysicsComponents.size(); }
			Handle getH() { return StaticPhysicsComponents[7]; }
		private:
			static Handle s_hInstance;
			// Store min max position of all objects
			std::vector<RectangularPrism> objectPositions;
			std::vector<Handle> StaticPhysicsComponents;
		};

		
	}

}
