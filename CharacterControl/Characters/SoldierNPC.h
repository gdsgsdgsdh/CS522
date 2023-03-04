#ifndef _CHARACTER_CONTROL_SOLDIER_NPC_
#define _CHARACTER_CONTROL_SOLDIER_NPC_

#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "PrimeEngine/Scene/PhysicsComponent.h"


#include "../Events/Events.h"

namespace CharacterControl{

namespace Components {

struct SoldierNPC : public PE::Components::Component
{
	PE_DECLARE_CLASS(SoldierNPC);

	SoldierNPC(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, Events::Event_CreateSoldierNPC *pEvt);
	virtual void addDefaultComponents();
	//SceneNode* getParentsSceneNode();
	char m_name[32];
	Matrix4x4 m_base;
};


}; // namespace Components
}; // namespace CharacterControl
#endif

