#pragma once
#include <glm\vec2.hpp>
#include "RigidBody.h"

class AABB : public RigidBody
{
public:
	AABB(glm::vec2 position);

	virtual bool checkCollision(PhysicsObject* pOther) { return false;  }
	virtual void makeGizmo();
	glm::vec2 GetExtents() const { return m_extents; }
	//glm::vec2 getWidth() const { return m_width;  }
	//glm::vec2 getheight() const { return m_height; }
	//glm::vec2 getLocalX() const { return m_LocalX; }
	//glm::vec2 getLocalY() const { return m_LocalY; }
	//glm::vec2 GetMoment() const { return m_getmoment; }

private:
	glm::vec2 m_extents = { 10,10 };

	//glm::vec2 m_width;
	//glm::vec2 m_height;
	//glm::vec2 m_LocalX;
	//glm::vec2 m_LocalY;
	//glm::vec2 m_getmoment;

 
};

