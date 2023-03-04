#include "PhysicsManager.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"

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
			PhysicsComponent::addDefaultComponents();

			//PE_REGISTER_EVENT_HANDLER(Events::Event_GATHER_DRAWCALLS, RootSceneNode::do_GATHER_DRAWCALLS);
			//PE_REGISTER_EVENT_HANDLER(Events::Event_GATHER_DRAWCALLS_Z_ONLY, RootSceneNode::do_GATHER_DRAWCALLS);

		}


		// Individual events -------------------------------------------------------
	}; // namespace Components
}; // namespace PE
