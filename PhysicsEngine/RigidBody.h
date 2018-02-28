#pragma once
#include "PhysicsObject.h"
class RigidBody : public PhysicsObject
{
public:
//RigidBody(const glm::vec2& position, float mass = 1.0f);
//~RigidBody();
	RigidBody(ShapeType shapeID, glm::vec2 position, 
		glm::vec2 velocity, float rotation, float mass);

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug(); 
	void applyforce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	
	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	virtual void ResetPoision();

    glm::vec2 getPosition() const { return m_position; }
	glm::vec2 SetPosition(glm::vec2 position) { m_position = position; return m_position;  }
	float getRotation()  { return m_rotation; } 
	glm::vec2 getVelocity() { return m_velocity; }
	glm::vec2 SetVelocity(glm::vec2 velocity) { m_velocity = velocity; return m_velocity; }
	float getMass() { return m_mass; }
protected:	
	float m_mass;
	glm::vec2 m_position;
	glm::vec2 m_Originalposition;

	glm::vec2 m_velocity;

	float m_rotation; 

	glm::vec2 m_acceleration; 
};

