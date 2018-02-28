#include "RigidBody.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include <iostream>



RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass)
	:PhysicsObject(shapeID)
	, m_mass(mass)
	, m_velocity(0, 0)
	, m_position(position)
	, m_Originalposition(position)
	,m_rotation(0)
	,m_acceleration(0,0)
{
	

}

void RigidBody::ResetPoision()
{
//	m_position = m_originalPosition; 
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
//	aie::Input* input = aie::Input::getInstance();
//	applyforce(gravity * m_mass * timeStep);
//	m_position += m_velocity * timeStep;


	m_velocity += gravity * timeStep;
 	m_velocity += m_acceleration * timeStep;
 	m_position += m_velocity * timeStep;
 
 	m_acceleration = { 0,0 };
}

void RigidBody::debug()
{
}

void RigidBody::applyforce(glm::vec2 force)
{
	// f = m*a 
	// a = f/m
	// v = a * t
	//m_velocity += acc; 

	glm::vec2 acc = force / m_mass;
	m_acceleration += acc; 
}

void RigidBody::applyForceToActor(RigidBody * actor2, glm::vec2 force)
{
//	applyforce(force / m_mass);
//	m_acceleration -= m_velocity; 
	aie::Input* input = aie::Input::getInstance();

 
 		applyforce(force / m_mass);
		actor2->applyforce(-force);

		
	
}

bool RigidBody::checkCollision(PhysicsObject * pOther)
{
	
	return false;
}
