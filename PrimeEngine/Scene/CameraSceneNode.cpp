#include "CameraSceneNode.h"
#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"

#define Z_ONLY_CAM_BIAS 0.0f
namespace PE {
namespace Components {

PE_IMPLEMENT_CLASS1(CameraSceneNode, SceneNode);

CameraSceneNode::CameraSceneNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself) : SceneNode(context, arena, hMyself)
{
	m_near = 0.05f;
	m_far = 2000.0f;
}
void CameraSceneNode::addDefaultComponents()
{
	Component::addDefaultComponents();
	PE_REGISTER_EVENT_HANDLER(Events::Event_CALCULATE_TRANSFORMATIONS, CameraSceneNode::do_CALCULATE_TRANSFORMATIONS);
}

void CameraSceneNode::do_CALCULATE_TRANSFORMATIONS(Events::Event *pEvt)
{
	Handle hParentSN = getFirstParentByType<SceneNode>();
	if (hParentSN.isValid())
	{
		Matrix4x4 parentTransform = hParentSN.getObject<PE::Components::SceneNode>()->m_worldTransform;
		m_worldTransform = parentTransform * m_base;
	}
	
	Matrix4x4 &mref_worldTransform = m_worldTransform;

	Vector3 pos = Vector3(mref_worldTransform.m[0][3], mref_worldTransform.m[1][3], mref_worldTransform.m[2][3]);
	Vector3 n = Vector3(mref_worldTransform.m[0][2], mref_worldTransform.m[1][2], mref_worldTransform.m[2][2]);
	Vector3 target = pos + n;
	Vector3 up = Vector3(mref_worldTransform.m[0][1], mref_worldTransform.m[1][1], mref_worldTransform.m[2][1]);

	m_worldToViewTransform = CameraOps::CreateViewMatrix(pos, target, up);

	m_worldTransform2 = mref_worldTransform;

	m_worldTransform2.moveForward(Z_ONLY_CAM_BIAS);

	Vector3 pos2 = Vector3(m_worldTransform2.m[0][3], m_worldTransform2.m[1][3], m_worldTransform2.m[2][3]);
	Vector3 n2 = Vector3(m_worldTransform2.m[0][2], m_worldTransform2.m[1][2], m_worldTransform2.m[2][2]);
	Vector3 target2 = pos2 + n2;
	Vector3 up2 = Vector3(m_worldTransform2.m[0][1], m_worldTransform2.m[1][1], m_worldTransform2.m[2][1]);

	m_worldToViewTransform2 = CameraOps::CreateViewMatrix(pos2, target2, up2);
    
    PrimitiveTypes::Float32 aspect = (PrimitiveTypes::Float32)(m_pContext->getGPUScreen()->getWidth()) / (PrimitiveTypes::Float32)(m_pContext->getGPUScreen()->getHeight());
    
    PrimitiveTypes::Float32 verticalFov = 0.33f * PrimitiveTypes::Constants::c_Pi_F32;
    if (aspect < 1.0f)
    {
        //ios portrait view
        static PrimitiveTypes::Float32 factor = 0.5f;
        verticalFov *= factor;
    }

	m_viewToProjectedTransform = CameraOps::CreateProjectionMatrix(verticalFov, 
		aspect,
		m_near, m_far);
	initCameraViewBoundaryPlanes(0.20f * PrimitiveTypes::Constants::c_Pi_F32, aspect, m_near, m_far);
	SceneNode::do_CALCULATE_TRANSFORMATIONS(pEvt);
	
}

void CameraSceneNode::initCameraViewBoundaryPlanes(PrimitiveTypes::Float32 verticalFov, PrimitiveTypes::Float32 aspect, float m_near, float m_far) {
	Matrix4x4 origin;
	origin.loadIdentity();
	origin.setPos(Vector3(0, 0, 0));	// use default position

	// init 6 planes
	for (auto& plane : m_cameraViewBoundaryPlanes) {
		plane = Matrix4x4(origin);
	}

	// Near plane only move forward
	m_cameraViewBoundaryPlanes[0].moveForward(m_near);

	// Far plane move forward and trun 180°
	m_cameraViewBoundaryPlanes[1].moveForward(m_far);
	m_cameraViewBoundaryPlanes[1].turnLeft(PrimitiveTypes::Constants::c_Pi_F32);

	
	PrimitiveTypes::Float32 leftAndRightPlaneTurnAngle = 0.5f * (PrimitiveTypes::Constants::c_Pi_F32 - verticalFov);
	// Left plane = camera plane's n turn right 90°- verticalFov / 2
	m_cameraViewBoundaryPlanes[2].turnRight(leftAndRightPlaneTurnAngle);
	// Right plane = camera plane's n turn left 90°- verticalFov / 2
	m_cameraViewBoundaryPlanes[3].turnLeft(leftAndRightPlaneTurnAngle);

	PrimitiveTypes::Float32 topAndBotPlaneTurnAngle = 0.5f * (PrimitiveTypes::Constants::c_Pi_F32 - verticalFov * aspect);
	// Top plane = camera plane's n turn down 90°- horizontal / 2
	m_cameraViewBoundaryPlanes[4].turnDown(topAndBotPlaneTurnAngle);
	// Bot plane = camera plane's n turn down 90°- horizontal / 2
	m_cameraViewBoundaryPlanes[5].turnUp(topAndBotPlaneTurnAngle);
}
}; // namespace Components
}; // namespace PE
