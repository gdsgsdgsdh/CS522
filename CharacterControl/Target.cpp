#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Lua/EventGlue/EventDataCreators.h"


#include "Target.h"


using namespace PE;
using namespace PE::Components;
using namespace CharacterControl::Events;

namespace CharacterControl{
namespace Events{

PE_IMPLEMENT_CLASS1(Event_CREATE_TARGET, PE::Events::Event);

void Event_CREATE_TARGET::SetLuaFunctions(PE::Components::LuaEnvironment *pLuaEnv, lua_State *luaVM)
{
	static const struct luaL_Reg l_Event_CREATE_TARGET[] = {
		{"Construct", l_Construct},
		{NULL, NULL} // sentinel
	};

	// register the functions in current lua table which is the table for Event_CreateSoldierNPC
	luaL_register(luaVM, 0, l_Event_CREATE_TARGET);
}

int Event_CREATE_TARGET::l_Construct(lua_State* luaVM)
{
    PE::Handle h("EVENT", sizeof(Event_CREATE_TARGET));
	Event_CREATE_TARGET *pEvt = new(h) Event_CREATE_TARGET;

	// get arguments from stack
	int numArgs, numArgsConst;
	numArgs = numArgsConst = 15;

	const char* targetName = lua_tostring(luaVM, -numArgs--);
	const char* name = lua_tostring(luaVM, -numArgs--);

	float positionFactor = 1.0f / 100.0f;
	Vector3 pos, u, v, n;
	pos.m_x = (float)lua_tonumber(luaVM, -numArgs--) * positionFactor;
	pos.m_y = (float)lua_tonumber(luaVM, -numArgs--) * positionFactor;
	pos.m_z = (float)lua_tonumber(luaVM, -numArgs--) * positionFactor;

	u.m_x = (float)lua_tonumber(luaVM, -numArgs--); u.m_y = (float)lua_tonumber(luaVM, -numArgs--); u.m_z = (float)lua_tonumber(luaVM, -numArgs--);
	v.m_x = (float)lua_tonumber(luaVM, -numArgs--); v.m_y = (float)lua_tonumber(luaVM, -numArgs--); v.m_z = (float)lua_tonumber(luaVM, -numArgs--);
	n.m_x = (float)lua_tonumber(luaVM, -numArgs--); n.m_y = (float)lua_tonumber(luaVM, -numArgs--); n.m_z = (float)lua_tonumber(luaVM, -numArgs--);

	pEvt->m_peuuid = LuaGlue::readPEUUID(luaVM, -numArgs--);

	

	// set data values before popping memory off stack
	StringOps::writeToString(targetName, pEvt->m_targetName, 32);
	StringOps::writeToString(name, pEvt->m_name, 32);
	
	lua_pop(luaVM, numArgsConst); //Second arg is a count of how many to pop

	pEvt->m_base.loadIdentity();
	pEvt->m_base.setPos(pos);
	pEvt->m_base.setU(u);
	pEvt->m_base.setV(v);
	pEvt->m_base.setN(n);

	LuaGlue::pushTableBuiltFromHandle(luaVM, h); 

	return 1;
}
};
namespace Components {

PE_IMPLEMENT_CLASS1(Target, Component);

// create Target form creation event
Target::Target(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, const Events::Event_CREATE_TARGET *pEvt)
: Component(context, arena, hMyself)
{
	StringOps::writeToString(pEvt->m_name, m_name, 32);
	StringOps::writeToString(pEvt->m_targetName, m_targetName, 32);

	m_base = pEvt->m_base;
}

void Target::addDefaultComponents()
{
	Component::addDefaultComponents();

	// custom methods of this component
}

}; // namespace Components
}; // namespace CharacterControl
