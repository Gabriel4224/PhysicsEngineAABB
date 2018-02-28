#pragma once
#include "PhysicsObject.h"
class Plane : public PhysicsObject
{
public:

	Plane(); 
//	Plane() = delete;
	Plane(glm::vec2 normal, float distance);

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) override {};
	virtual void debug(){}
	virtual void makeGizmo() override;
	virtual void resetPosition(); 

	glm::vec2 getNormal () const { return m_normal; }
	float getDistance()const { return m_distanceToOrigin;  }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin; 
};

