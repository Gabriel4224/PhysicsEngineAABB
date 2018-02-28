#include "Plane.h"




Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);

}

Plane::Plane(glm::vec2 normal, float distance)
	:PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
}


void Plane::makeGizmo()
{
	
	const float lineSegmentLength = 300;
	glm::vec2 center = m_normal * -m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, m_normal.x);
	
	glm::vec4 Colour(0, 1, 0, 1);

	glm::vec2 start = center + (parallel * lineSegmentLength);
	glm::vec2 end = center - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, Colour);

}

void Plane::resetPosition()
{
	getNormal(); 
}
