#include "PhysicsManager.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include <string.h>
#include "PrimeEngine/Utils/StringOps.h"

using namespace std;
namespace PE {
	namespace Components {

		Handle PhysicsManager::s_hInstance;
		PE_IMPLEMENT_CLASS1(PhysicsManager, Component);

		void PhysicsManager::Construct(PE::GameContext& context, PE::MemoryArena arena)
		{
			Handle h("PhysicsManager", sizeof(PhysicsManager));
			PhysicsManager* pPhysicsManager = new(h) PhysicsManager(context, arena, h);
			pPhysicsManager->addDefaultComponents();
			SetInstance(h);

			/*s_hTitleInstance = Handle("ROOT_SCENE_NODE", sizeof(RootSceneNode));
			RootSceneNode* pTitleRootSceneNode = new(s_hTitleInstance) RootSceneNode(context, arena, h);
			pTitleRootSceneNode->addDefaultComponents();

			SetTitleAsCurrent();*/
		}

		// Component ------------------------------------------------------------

		

		void PhysicsManager::addDefaultComponents() {
			Component::addDefaultComponents();
			//PE_REGISTER_EVENT_HANDLER(Events::Event_GATHER_DRAWCALLS, RootSceneNode::do_GATHER_DRAWCALLS);
			//PE_REGISTER_EVENT_HANDLER(Events::Event_GATHER_DRAWCALLS_Z_ONLY, RootSceneNode::do_GATHER_DRAWCALLS);

		}

		std::vector<Vector3> PhysicsManager::calculate_normals(std::vector<Vector3> vertices) {
			// Use verticves build 6 faces. Numbers in the vector represent the index of the vertices in vector"vertices"
			std::vector<std::vector<int>> faces = { {0,1,5,4}, {1,3,7,5}, {4,5,7,6}, {0,2,6,4}, {0,1,3,2}, {2,3,7,6} };
			std::vector<Vector3> normals;
			for (auto &face : faces) {
				Vector3 edge1 = vertices[face[1]] - vertices[face[0]];
				Vector3 edge2 = vertices[face[2]] - vertices[face[0]];
				Vector3 normal = edge1.crossProduct(edge2);
				normal.normalize();
				normals.emplace_back(normal);
			}
			return normals;
		}

		// Project a rectangular prism onto an axis and return the range
		std::pair<float, float> PhysicsManager::project_onto_axis(const RectangularPrism& rp, Vector3 axis) {
			float min_proj = axis.dotProduct(rp.vertices[0]);
			float max_proj = min_proj;
			for (int i = 1; i < 8; i++) {
				float proj = axis.dotProduct(rp.vertices[i]);
				if (proj < min_proj) {
					min_proj = proj;
				}
				else if (proj > max_proj) {
					max_proj = proj;
				}
			}
			return std::make_pair(min_proj, max_proj);
		}

		// Check if two ranges overlap
		bool overlap(float min1, float max1, float min2, float max2) {
			return std::max(min1, max1) >= std::min(max2, min2);
		}

		// Check if two rectangular prisms intersect
		bool PhysicsManager::Intersect(RectangularPrism RpA, RectangularPrism RpB) {
			for (const auto &normal : RpA.normals) {
				auto range1 = project_onto_axis(RpA, normal);
				auto range2 = project_onto_axis(RpB, normal);
				if (!overlap(range1.first, range1.second, range2.first, range2.second)) {
					return false;
				}
			}
			for (const auto &normal : RpB.normals) {
				auto range1 = project_onto_axis(RpA, normal);
				auto range2 = project_onto_axis(RpB, normal);
				if (!overlap(range1.first, range1.second, range2.first, range2.second)) {
					return false;
				}
			}

			vector<int> indexs{ 0, 2, 3 };
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					auto normal = RpA.normals[i].crossProduct(RpB.normals[j]);
					auto range1 = project_onto_axis(RpA, normal);
					auto range2 = project_onto_axis(RpB, normal);
					if (!overlap(range1.first, range1.second, range2.first, range2.second)) {
						return false;
					}
				}
			}
			return true;

		}

		void PhysicsManager::addObjectPosition(std::vector<Vector3> vertices, Matrix4x4 m_base) {
			std::vector<Vector3> normals = calculate_normals(vertices);
			RectangularPrism curInst;
			curInst.vertices = vertices;
			curInst.normals = normals;
			curInst.m_base = m_base;
			objectPositions.emplace_back(curInst);
		}

		void PhysicsManager::updateVertices(std::vector<Vector3>& vertices, const Vector3& dir, const float& dist) {
			for (auto& vertex : vertices) {
				vertex += dist * dir;
			}
		}

		void PhysicsManager::updatePos(Vector3& pos, const Vector3& dir, const float& dist) {
			pos += dist * dir;
		}

		void PhysicsManager::updatePosition(RectangularPrism& curInst, const Vector3& dir) {
			// Project vertices of a onto dir
			float aMin = std::numeric_limits<float>::max();
			float aMax = std::numeric_limits<float>::min();

			for (auto& vertex : curInst.vertices) {
				float projection = vertex.getX() * dir.getX() + vertex.getY() * dir.getY() + vertex.getZ() * dir.getZ();
				aMin = std::min(aMin, projection);
				aMax = std::max(aMax, projection);
			}
			/*float aMin = 0.2;
			float aMax = 0;*/
			// Move a along dir by amount equal to penetration depth
			Vector3 translation = (aMax - aMin) * dir;
			curInst.m_base.setPos(curInst.m_base.getPos() + translation);

			// Update vertices of a
			for (auto& vertex : curInst.vertices) {
				vertex += translation;
			}
		}

		bool PhysicsManager::checkIfCollison(std::vector<Vector3> vertices, Vector3 m_base, Vector3& dir, float dist) {
			std::vector<Vector3> normals = calculate_normals(vertices);
			RectangularPrism curInst;
			curInst.vertices = vertices;
			curInst.normals = normals;
			Vector3 initPos = m_base;
			bool collisionDetected = false;
			for (int i = 0; i < objectPositions.size(); ++i) {
			// for (auto& inst : objectPositions) {
				if (Intersect(curInst, objectPositions[i])) {
					char buffer[10];
					collisionDetected = true;
					// Collision found, have to update direction to avoid colliding
					UpdateDir(curInst, objectPositions[i], dir);
					updatePosition(curInst, dir);
					return true;
				}
			}
			return false;
		}

		// Return the index of the normals. Which has the shorted distance to point
		int PhysicsManager::distanceToPointToLevel(Vector3 point, RectangularPrism rP) {
			auto normals = rP.normals;
			double shortestDist = 9999;
			int levelIndex = 0;
			for (int i = 0; i < 6; ++i) {
				double d = normals[i].dotProduct(rP.vertices[rP.faces[i][0]]);
				double distance = abs(normals[i].dotProduct(point) - d) / normals[i].length();
				if (distance < shortestDist) {
					shortestDist = distance;
					levelIndex = i;
				}
			}
			return 2;
			
		}

		// Moves RpA in the direction of dir until it no longer collides with RpB
		void PhysicsManager::UpdateDir(const RectangularPrism& RpA, const RectangularPrism& RpB, Vector3& dir) {
			
			Vector3 diff = RpB.m_base.getPos() - RpA.m_base.getPos();
			diff.normalize();
			dir = dir - diff;
			dir.normalize();
			return;
		}

		bool PhysicsManager::checkPhyscisComponents(Handle h) {
			for (int i = 0; i < StaticPhysicsComponents.size(); ++i) {
				if (h == StaticPhysicsComponents[i]) {
					return true;
				}
			}
			return false;
		}
		// Individual events -------------------------------------------------------
	}; // namespace Components
}; // namespace PE
