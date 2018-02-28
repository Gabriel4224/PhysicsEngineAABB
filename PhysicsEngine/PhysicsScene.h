#pragma once
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <glm\vec2.hpp>
#include <vector>

class PhysicsObject;
class PhysicsScene
{
public:
	struct collisiondata
{
		bool wasCollision;
		glm::vec2 normal;
		float overlap;
		//glm::vec2 Contactpoint; 
	};
public:
	PhysicsScene();
	~PhysicsScene();
    void addActor(PhysicsObject* actor);
    void removeActor(PhysicsObject* actor);
    void update(float dt);
    void updateGizmos();
    void debugScene(); 
	void handleCollision(PhysicsObject* object1, PhysicsObject* object2, const collisiondata& Collision);
	void SeperatCollisionObjects(PhysicsObject*object1, PhysicsObject* object2 , const collisiondata& Collision);
    void CheckForCollision();

	void setGravity(const glm::vec2 gravity)
	{
		m_gravity = gravity; 
	}
	glm::vec2 getGravity() const{return m_gravity;}
	void setTimeStep(const float timeStep)
	{	m_timeStep = timeStep;}	
	float getTimeStep() const{return m_timeStep; }

	static collisiondata Plane2Sphere(const class PhysicsObject* object1, const class PhysicsObject* object2);
	static collisiondata Sphere2PLane(const class PhysicsObject* object1, const class PhysicsObject* object2);
	static collisiondata Sphere2Sphere(const class PhysicsObject* object1, const class PhysicsObject* object2);
	static collisiondata AABB2Plane(const class PhysicsObject* object1, const class PhysicsObject* object2);
	static collisiondata AABB2Sphere(const class PhysicsObject* object1, const class PhysicsObject* object2);

	static collisiondata Plane2AABB(const class PhysicsObject* object1, const class PhysicsObject* object2);
	static collisiondata Sphere2AABB(const class PhysicsObject* object1, const class PhysicsObject* object2);

	static collisiondata AABB2AABB(const class PhysicsObject* object1, const class PhysicsObject* object2);


protected:
	glm::vec2 m_gravity; 
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

