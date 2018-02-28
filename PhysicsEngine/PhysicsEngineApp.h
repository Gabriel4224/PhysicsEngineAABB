#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
class PhysicsEngineApp : public aie::Application {
public:

	PhysicsEngineApp();
	virtual ~PhysicsEngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	PhysicsScene *m_physicsScene;
	RigidBody *rb;
	PhysicsObject *m_PhysicsObject; 
	PhysicsObject *object1;
	PhysicsObject *object2;

	Sphere *m_sphere;
	Plane *m_plane; 
	Plane *m_plane1;
	Plane *m_plane2;
	Plane *m_plane3;
	AABB *m_test; 
	AABB *m_test1;

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};