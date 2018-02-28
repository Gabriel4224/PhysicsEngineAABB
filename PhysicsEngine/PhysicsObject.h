#pragma once
#include "PhysicsScene.h"


class PhysicsObject
{
public:
enum ShapeType
{
	PLANE = 0,
	SPHERE,
	AABB,
	SHAPE_COUNT,
};
	PhysicsObject() {}

	PhysicsObject(ShapeType a_shapeID) : m_shape(a_shapeID) {}
	
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0; 
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {}; 

	ShapeType getShapeID() { return m_shape; }
protected:
	ShapeType m_shape; 
};

