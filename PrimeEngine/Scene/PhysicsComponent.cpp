#include "PhysicsComponent.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "SceneNode.h"
#include "SkeletonInstance.h"
#include "DebugRenderer.h"


namespace PE {
	namespace Components {

		// Constructor -------------------------------------------------------------
		PhysicsComponent::PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself) : Component (context, arena, hMyself) {

		}


		// Component ------------------------------------------------------------
		void PhysicsComponent::addDefaultComponents()
		{
			/*SceneNode::addDefaultComponents();*/


			// Data components

			// event handlers
			/*PE_REGISTER_EVENT_HANDLER(Events::Event_MOVE, SceneNode::do_MOVE);
			PE_REGISTER_EVENT_HANDLER(Events::Event_CALCULATE_TRANSFORMATIONS, SceneNode::do_CALCULATE_TRANSFORMATIONS);*/

		}

		void PhysicsComponent::createAABB(MeshInstance* pInst, Mesh* pMeshCaller) {
			Handle hParentSN = pInst->getFirstParentByType<SceneNode>();
			if (!hParentSN.isValid()) {
				hParentSN = pInst->getFirstParentByTypePtr<SkeletonInstance>()->getFirstParentByType<SceneNode>();
			}
			Matrix4x4 worldMatrix;
			Vector3 m_pos;
			if (hParentSN.isValid()) {
				SceneNode* pSN = hParentSN.getObject<SceneNode>();
				worldMatrix = pSN->m_worldTransform;
				m_pos = pSN->m_base.getPos();
			}
			PositionBufferCPU* pPoss = pMeshCaller->m_hPositionBufferCPU.getObject<PositionBufferCPU>();
			Vector3 minPos = pPoss->minPos;
			Vector3 maxPos = pPoss->maxPos;
			Vector3 vertices[8];
			generateVertexForAABB(minPos, maxPos, vertices);
			DebugRenderer::Instance()->drawAABB(vertices, worldMatrix);

		}




		void PhysicsComponent::generateVertexForAABB(const Vector3& min, const Vector3& max, Vector3 vertices[8]) {

			vertices[0] = Vector3(min.m_x, min.m_y, min.m_z);
			vertices[1] = Vector3(min.m_x, min.m_y, max.m_z);
			vertices[2] = Vector3(min.m_x, max.m_y, min.m_z);
			vertices[3] = Vector3(min.m_x, max.m_y, max.m_z);
			vertices[4] = Vector3(max.m_x, min.m_y, min.m_z);
			vertices[5] = Vector3(max.m_x, min.m_y, max.m_z);
			vertices[6] = Vector3(max.m_x, max.m_y, min.m_z);
			vertices[7] = Vector3(max.m_x, max.m_y, max.m_z);

		}

		// Individual events -------------------------------------------------------
	}; // namespace Components
}; // namespace PE
