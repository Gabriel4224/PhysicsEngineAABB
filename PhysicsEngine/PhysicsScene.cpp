 #include "PhysicsScene.h"
#include <list>
#include "RigidBody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto& actor : m_actors)
	{
		delete actor; 
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
			pActor->makeGizmo();
		}
		accumulatedTime -= m_timeStep;

		//for (auto pActor : m_actors) {
		//	for (auto pOther : m_actors) {
		//		if (pActor == pOther)
		//			continue;
		//		if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
		//			std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
		//			continue;
		//
		//		 RigidBody* pRigid = dynamic_cast<RigidBody*>(pActor);
		//		if (pRigid->checkCollision(pOther) == true) {
		//			pRigid->applyForceToActor(
		//				dynamic_cast<RigidBody*>(pOther),
		//				pRigid->getVelocity() * pRigid->getMass());
		//			dirty.push_back(pRigid);
		//			dirty.push_back(pOther);
		//		}
		//
		//	}
		//}
		//dirty.clear();
		CheckForCollision();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::handleCollision(PhysicsObject * object1, PhysicsObject * object2, 
	const collisiondata & collsion)
{

	auto rb1 = dynamic_cast<RigidBody*>(object1);
	auto rb2 = dynamic_cast<RigidBody*>(object2);

	SeperatCollisionObjects(rb1, rb2, collsion); 

	glm::vec2 relativeVelocity = { 0,0 };
	if (rb1) relativeVelocity = rb1->getVelocity();
	if (rb2) relativeVelocity -= rb2->getVelocity();
	float elasticity = 1;

	float invmass1 = (rb1) ? 1.0f / rb1->getMass() : 0.0f; 
	float invmass2 = (rb2) ? 1.0f / rb2->getMass() : 0.0f;

	float jTop = -(1 + elasticity) * glm::dot(relativeVelocity, collsion.normal);
	float jBottom = glm::dot(collsion.normal, collsion.normal) * (invmass1 + invmass2);

	float j = jTop / jBottom; 

	if (rb1)
	{
		glm::vec2 newVelocity = rb1->getVelocity() + (j / rb1->getMass())
			* collsion.normal;
	
		rb1->SetVelocity(newVelocity);
	}
	if (rb2)
	{
		glm::vec2 newVelocity = rb2->getVelocity() - (j / rb2->getMass())
			* collsion.normal;

		rb2->SetVelocity(newVelocity);
	}

}

void PhysicsScene::SeperatCollisionObjects(PhysicsObject* object1, PhysicsObject* object2 ,const  collisiondata& Collision)
{
	auto rb1 = dynamic_cast<RigidBody*>(object1);
	auto rb2 = dynamic_cast<RigidBody*>(object2);

	if (rb1 == nullptr && rb2 == nullptr)
	{
		return;
	}

	float object1moveratio = 0;
	float object2moveratio = 0;
	if (rb1 && rb2)
	{

		float totalmass = rb1->getMass() + rb2->getMass();
		object1moveratio = 1 - (rb1->getMass() / totalmass);
		object2moveratio = 1 - (rb2->getMass() / totalmass);

	}
	else if (rb1)
	{
		object1moveratio = 1.0f;
	}
	else if (rb2)
	{
		object2moveratio = 1.0f;

	}

	if (rb1)
	{
		rb1->SetPosition(rb1->getPosition() - (object1moveratio *
			Collision.overlap * Collision.normal));
	}
	if (rb2)
	{
		rb2->SetPosition(rb2->getPosition() + (object2moveratio *
			Collision.overlap * Collision.normal));
	}
}



typedef PhysicsScene::collisiondata(*collisionFnc)(const PhysicsObject*, const PhysicsObject*);

static collisionFnc collisionFunctionArray[] = {
	nullptr, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2AABB,
	PhysicsScene::Sphere2PLane , PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2AABB,
	PhysicsScene::AABB2Plane , PhysicsScene::AABB2Sphere, PhysicsScene::AABB2AABB 
};

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size(); 
	// checks for collisons against all object except itself
	for (int outer = 0; outer != actorCount; outer++)
	{
		for (int inner = outer + 1; inner != actorCount; inner++)
		{
			auto object1 = m_actors[outer];
			auto object2 = m_actors[inner];
			
			int ShapeID1 = object1->getShapeID();
			int ShapeID2 = object2->getShapeID();

			int index = (ShapeID1 * PhysicsObject::ShapeType::SHAPE_COUNT) + ShapeID2;

			collisionFnc fnc = collisionFunctionArray[index];
			if (fnc != nullptr)
			{
				collisiondata Collisiondata = fnc(object1, object2);
				if (Collisiondata.wasCollision == true)
				{
					handleCollision(object1, object2, Collisiondata);
				}
			}

		}
	}
}

PhysicsScene::collisiondata PhysicsScene::Plane2Sphere(const PhysicsObject * object1, const PhysicsObject * object2)
{
	return Sphere2PLane(object2, object1);
}

PhysicsScene::collisiondata PhysicsScene::Sphere2PLane(const PhysicsObject * object1, const PhysicsObject * object2)
{
	const Sphere *sphere = dynamic_cast<const Sphere*> (object1);
	const Plane *plane = dynamic_cast<const Plane*> (object2);

	collisiondata coldata;
	coldata.wasCollision = false;

	if (sphere != nullptr &&  plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float spheretoplane = glm::dot(
			sphere->getPosition(),
			plane->getNormal()) + plane->getDistance();


		if (spheretoplane < 0)
		{
			collisionNormal *= 1;
			spheretoplane *= 1;
		}
		float intersection = sphere->getRadius() - spheretoplane;
		if (intersection > 0)
		{
			std::cout << "PlaneCollison" << std::endl;
			coldata.wasCollision = true;
			coldata.normal = plane->getNormal();
			coldata.overlap = intersection;
		}
	}

	return coldata;
}

PhysicsScene::collisiondata  PhysicsScene::Sphere2Sphere(const PhysicsObject * object1, const PhysicsObject * object2)
{
	const Sphere *sphere1 = dynamic_cast<const Sphere*>(object1);
	const Sphere *sphere2 = dynamic_cast<const Sphere*>(object2);
	assert(sphere1 && sphere2);


	collisiondata coldata;
	coldata.wasCollision = false;


	// distance equals sphere 1 take sphere 2
						  // sphere 1 (x1 ,y1)    // sphere 2 (x2,y2)
	glm::vec2 distance = sphere1->getPosition() - sphere2->getPosition();
	if (glm::length(distance) <= sphere1->getRadius() + sphere2->getRadius())
	{
		// x^ + y^ = 
		//collision  
		std::cout << "Collision" << std::endl;
		coldata.wasCollision = true;
		coldata.normal = glm::normalize(sphere2->getPosition() - sphere1->getPosition());
		coldata.overlap = (sphere1->getRadius() + sphere2->getRadius()) - glm::length(distance);
	}


	return coldata;
}
// AABB COLLIDES WITH PLANE 
PhysicsScene::collisiondata  PhysicsScene::AABB2Plane(const PhysicsObject * object1, const PhysicsObject * object2)
{
	const AABB *aabb = dynamic_cast<const AABB*> (object1);
	const Plane *plane = dynamic_cast<const Plane*> (object2);
	collisiondata coldata;
	coldata.wasCollision = false;
	

	if (aabb != nullptr &&  plane != nullptr)
	{
		
		// top right corner of aabb 
		glm::vec2 tr = aabb->getPosition() + glm::vec2(aabb->GetExtents().x, aabb->GetExtents().y);
		// top left corner of aabb 
		glm::vec2 tl = aabb->getPosition() + glm::vec2(-aabb->GetExtents().x, aabb->GetExtents().y);
		// bottom right corner of aabb 
		glm::vec2 br = aabb->getPosition() + glm::vec2(aabb->GetExtents().x, -aabb->GetExtents().y);
		// bottom left corner of aabb 
		glm::vec2 bl = aabb->getPosition() + glm::vec2(-aabb->GetExtents().x, -aabb->GetExtents().y);
		
		float tr_distance = glm::dot(tr, plane->getNormal()) + plane->getDistance();
		float tl_distance = glm::dot(tl, plane->getNormal()) + plane->getDistance();
		float br_distance = glm::dot(br, plane->getNormal()) + plane->getDistance();
		float bl_distance = glm::dot(bl, plane->getNormal()) + plane->getDistance();
		//float smallest = <one of the four>
		float smallest = tr_distance;
		if (tl_distance < smallest)
		{
			smallest = tl_distance;
		}
		if (br_distance < smallest)
		{
			smallest = br_distance; 
		}
		if (bl_distance < smallest)
		{
			smallest = bl_distance;
		}
		if (smallest < 0)
		{
			std::cout << "AABB COLLISON TO PLANE" << std::endl; 
			coldata.wasCollision = true; 
			coldata.normal = glm::normalize(aabb->getPosition() - plane->getNormal());
			coldata.overlap = smallest;
		}
	}
	return coldata;
}

PhysicsScene::collisiondata  PhysicsScene::Plane2AABB(const PhysicsObject * object1, const PhysicsObject * object2)
{
	return AABB2Plane(object2, object1);

}
PhysicsScene::collisiondata  PhysicsScene::AABB2Sphere(const PhysicsObject * object1, const PhysicsObject * object2)
{
	const AABB *aabb = dynamic_cast<const AABB*>(object1);
	const Sphere *sphere = dynamic_cast<const Sphere*>(object2);
	assert(aabb && sphere);


	collisiondata coldata;
	coldata.wasCollision = false;
	//if (glm::length(distance) <= aabb->GetExtents() + sphere->getRadius())
	if (aabb != nullptr && sphere != nullptr)
	{
		//float tr_distance = glm::dot(tr, sphere->getPosition()) + sphere->getRadius();
		//float tl_distance = glm::dot(tl, sphere->getPosition()) + sphere->getRadius();
		//float br_distance = glm::dot(br, sphere->getPosition()) + sphere->getRadius();
		//float bl_distance = glm::dot(bl, sphere->getPosition()) + sphere->getRadius();
		glm::vec2 offset = sphere->getPosition() - aabb->getPosition();
	


		//clamping x axis 
		if (std::abs(offset.x) > aabb->GetExtents().x)
		{
			if (offset.x < 0)
			{
				offset.x = -aabb->GetExtents().x;
			}
			else
			{
				offset.x = aabb->GetExtents().x; 
			}
			
		}
		
		//clamping y axis
		if (std::abs(offset.y) > aabb->GetExtents().y)
		{
			if (offset.y < 0)
			{
				offset.y = -aabb->GetExtents().y;
			}
			else			
			{
				offset.y = aabb->GetExtents().y;
			}
		}

		glm::vec2 ClosestPOint = offset + aabb->getPosition();
		glm::vec2  offsetdistance = sphere->getPosition() - ClosestPOint;
		
		if (glm::length(offsetdistance) < sphere->getRadius())
		{
			std::cout << "AABB to SPHERE COLLISION" << std::endl;
			coldata.wasCollision = true;
			coldata.normal = glm::normalize(offsetdistance);
			coldata.overlap = sphere->getRadius() - glm::length(offsetdistance);
		}
		//if (sphere->getPosition().x >= aabb->getPosition().x && sphere->getPosition().y >= aabb->getPosition().y
		//	)
		//{
		//
		//}
		
		//float smallest = tr_distance;
		//if (tl_distance < smallest)
		//{
		//	smallest = tl_distance;
		//}
		//if (br_distance < smallest)
		//{
		//	smallest = br_distance;
		//}
		//if (bl_distance < smallest)
		//{
		//	smallest = bl_distance;
		//}
		//if (smallest < 0)
		//{
		//	std::cout << "AABB to SPHERE COLLISION" << std::endl;
		//	coldata.wasCollision = true;
		//	coldata.normal = glm::normalize(aabb->getPosition() - sphere->getPosition());
		//	coldata.overlap = smallest;
		//}
	//}
 return coldata;
//}
	}
}


PhysicsScene::collisiondata  PhysicsScene::Sphere2AABB(const PhysicsObject * object1, const PhysicsObject * object2)
{

	return AABB2Sphere(object2, object1);

}

PhysicsScene::collisiondata  PhysicsScene::AABB2AABB(const PhysicsObject * object1, const PhysicsObject * object2)
{
  const AABB *aabb1 = dynamic_cast<const AABB*>(object1);
  const AABB *aabb2 = dynamic_cast<const AABB*>(object2);
	
	collisiondata coldata;
	coldata.wasCollision = false;
	if (aabb1 != nullptr && aabb2 != nullptr)
	{
		//glm::vec2 tr = aabb1->getPosition() + glm::vec2(aabb1->GetExtents().x, aabb1->GetExtents().y);
		//glm::vec2 tl = aabb1->getPosition() + glm::vec2(-aabb1->GetExtents().x, aabb1->GetExtents().y);
		//glm::vec2 br = aabb1->getPosition() + glm::vec2(aabb1->GetExtents().x, -aabb1->GetExtents().y);
		//glm::vec2 bl = aabb1->getPosition() + glm::vec2(-aabb1->GetExtents().x, -aabb1->GetExtents().y);
		//
		//float tr_distance = glm::dot(tr, aabb2->getPosition()) + aabb2->GetExtents();
		//float tl_distance = glm::dot(tl, aabb2->getPosition()) + aabb2->GetExtents();
		//float br_distance = glm::dot(br, aabb2->getPosition()) + aabb2->GetExtents();
		//float bl_distance = glm::dot(bl, aabb2->getPosition()) + aabb2->GetExtents();

		glm::vec2 offset = aabb1->getPosition() - aabb2->getPosition();
		
		if (std::abs(offset.x) > aabb2->GetExtents().x)
		{
			if (offset.x < 0)
			{
				offset.x = -aabb2->GetExtents().x;
			}
			else
			{
				offset.x = aabb2->GetExtents().x;
			}
		}
		if (std::abs(offset.y) > aabb2->GetExtents().y)
		{
			if (offset.y < 0)
			{
				offset.y = -aabb2->GetExtents().y;
			}
			else
			{
				offset.y = aabb2->GetExtents().y;
			}
		}

	//glm::vec2 ClosestPOint = offset + aabb->getPosition();
	//glm::vec2  offsetdistance = sphere->getPosition() - ClosestPOint;
		glm::vec2 closestPoint = aabb1->getPosition() + offset   ;
		glm::vec2 overlap = aabb2->getPosition() - closestPoint; 
		
		if (glm::length(overlap) < glm::length(aabb2->GetExtents()))
//		if (glm::length(overlap) < glm::vec2(aabb2->GetExtents()))
		{
			std::cout << "AABB COLLISION WITH AABB" << std::endl;
			coldata.wasCollision = true;
			coldata.normal = glm::normalize(overlap);
			//coldata.overlap = glm::vec2(aabb2->GetExtents()) - glm::length(overlap);
		}
	}
//glm::vec2 distance = aabb1->getPosition() - aabb2->getPosition();
//
//if (glm::length(distance) <= glm::length(aabb1->GetExtents() - glm::length(aabb2->GetExtents())))
//{
//	
//	//std::cout << "AABBCOLLIDEDWITHAABB" << std::endl;
//	coldata.wasCollision = true;
//	coldata.normal = glm::normalize(aabb2->getPosition() + aabb1->getPosition());
//}
	
	return coldata;
}
