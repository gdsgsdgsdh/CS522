#ifndef __PYENGINE_2_0_CAMERA_SCENE_NODE_H__
#define __PYENGINE_2_0_CAMERA_SCENE_NODE_H__

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/Render/IRenderer.h"
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Utils/Array/Array.h"
#include "PrimeEngine/Math/CameraOps.h"

#include "SceneNode.h"


// Sibling/Children includes

namespace PE {
namespace Components {

struct CameraSceneNode : public SceneNode
{

	PE_DECLARE_CLASS(CameraSceneNode);

	// Constructor -------------------------------------------------------------
	CameraSceneNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);

	virtual ~CameraSceneNode(){}

	// Component ------------------------------------------------------------
	virtual void addDefaultComponents();

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CALCULATE_TRANSFORMATIONS);
	virtual void do_CALCULATE_TRANSFORMATIONS(Events::Event *pEvt);

	void initCameraViewBoundaryPlanes(PrimitiveTypes::Float32 verticalFov, PrimitiveTypes::Float32 aspect, float m_near, float m_far);
	// Individual events -------------------------------------------------------
	
	Matrix4x4 m_worldToViewTransform; // objects in world space are multiplied by this to get them into camera's coordinate system (view space)
	Matrix4x4 m_worldToViewTransform2;
	Matrix4x4 m_worldTransform2;
	Matrix4x4 m_viewToProjectedTransform; // objects in local (view) space are multiplied by this to get them to screen space
	Matrix4x4 m_cameraViewBoundaryPlanes[6]; // 0 for near plane, 1 for far plane, 2 for left, 3 for right, 4 for top, 5 for bot
	float m_near, m_far;
};
}; // namespace Components
}; // namespace PE
#endif
