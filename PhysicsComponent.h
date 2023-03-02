#pragma once
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"
#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"


namespace PE {
	namespace Components {
		struct PhysicsComponent : public Component
		{
			PE_DECLARE_CLASS(PhysicsComponent);
			// Constructor -------------------------------------------------------------
			PhysicsComponent();

			virtual ~PhysicsComponent() {}

			// Component ------------------------------------------------------------

			virtual void addDefaultComponents();

			// Individual events -------------------------------------------------------
		};

		
	}

}
