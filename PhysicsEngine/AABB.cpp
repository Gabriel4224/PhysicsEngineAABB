#include "AABB.h"



AABB::AABB(glm::vec2 position)
	: RigidBody(PhysicsObject::AABB, position, {0, 0}, 0, 1.0f)
{
}


void AABB::makeGizmo()
{
	aie::Gizmos:: add2DAABB(m_position, m_extents, glm::vec4(1, 0, 0, 1), nullptr);
}


