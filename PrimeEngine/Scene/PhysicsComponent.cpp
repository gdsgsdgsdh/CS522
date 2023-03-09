#include "PhysicsComponent.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "SceneNode.h"
#include "SkeletonInstance.h"
#include "DebugRenderer.h"
using namespace PE::Events;

namespace PE {
	namespace Components {

		PE_IMPLEMENT_CLASS1(PhysicsComponent, Component);

		// Constructor -------------------------------------------------------------
		PhysicsComponent::PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself) : Component (context, arena, hMyself) {

		}


		// Component ------------------------------------------------------------
		void PhysicsComponent::addDefaultComponents()
		{
			Component::addDefaultComponents();

			// Data components

			// event handlers
			/*PE_REGISTER_EVENT_HANDLER(Events::Event_MOVE, SceneNode::do_MOVE);
			PE_REGISTER_EVENT_HANDLER(Events::Event_CALCULATE_TRANSFORMATIONS, SceneNode::do_CALCULATE_TRANSFORMATIONS);*/

		}

		void PhysicsComponent::createAABB() {
			Handle hParentSN = getFirstParentByType<SceneNode>();
			Matrix4x4 worldMatrix;
			Vector3 m_pos;
			if (hParentSN.isValid())
			{
				SceneNode* pSN = hParentSN.getObject<SceneNode>();
				worldMatrix = pSN->m_worldTransform;
				m_pos = pSN->m_base.getPos();
			}
			
			// if (hParentSN.isValid()) {
				
			// }
			if (sides.size() > 0) {
				DebugRenderer::Instance()->drawAABB(sides, worldMatrix);
			}
			

		}


		void PhysicsComponent::setVerticesForAABB(std::vector<std::pair<Vector3, Vector3>> sides) {
			this->sides = sides;
		}
		// Individual events -------------------------------------------------------
	}; // namespace Components
}; // namespace PE
