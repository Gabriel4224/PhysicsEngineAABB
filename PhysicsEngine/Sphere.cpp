#include "Sphere.h"





Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
	:RigidBody(SPHERE,position,velocity,0,mass)
{
	m_radius = radius; 
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(getPosition(), m_radius, 32, glm::vec4(0, 1, 1, 1));

}

bool Sphere::checkCollision(PhysicsObject * pOther)
{


	return false;
}
