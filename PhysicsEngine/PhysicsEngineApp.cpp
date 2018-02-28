#include "PhysicsEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include <glm\vec2.hpp>
#include "Sphere.h"
#include "Plane.h"

#include <random>

PhysicsEngineApp::PhysicsEngineApp() {

}

PhysicsEngineApp::~PhysicsEngineApp() {

}

bool PhysicsEngineApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	
	m_physicsScene = new PhysicsScene();
	//collision = new PhysicsScene::collisiondata(); 
	//m_physicsScene->setGravity(glm::vec2(0, -20));
	m_physicsScene->setGravity(glm::vec2(0,0));

	m_physicsScene->setTimeStep(0.01f); 

  //  Sphere* ball;
  //  ball = new Sphere(glm::vec2(20, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
 //   m_physicsScene->addActor(ball);
	//int radius = (rand() % 10) + 1;
	//int Velocity = (rand() % 40) + 1;

	// creates the frist spheres location, velocity, mass,radius and colour
	Sphere* ball1 = new Sphere(glm::vec2(0, 33), glm::vec2(0, 0), 4.0f, 4.0f, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(0, 14), glm::vec2(0, 0), 4.0f, 4.0f, glm::vec4(1, 0, 0, 1));
	Sphere* ball3 = new Sphere(glm::vec2(15, 0), glm::vec2(0, 0), 4.0f, 4.0f, glm::vec4(1, 0, 0, 1));
	Sphere* ball4 = new Sphere(glm::vec2(0, 20), glm::vec2(0, 0), 4.0f, 4.0f, glm::vec4(1, 0, 0, 1));
	Sphere* ball5 = new Sphere(glm::vec2(4, 4), glm::vec2(0, 0), 4.0f, 4.0f, glm::vec4(1, 0, 0, 1));
	Sphere* ball6 = new Sphere(glm::vec2(8, 4), glm::vec2(0, 0), 4.0f, 4.0f, glm::vec4(1, 0, 0, 1));
	
	m_test = new AABB(glm::vec2(-20, 0));
	m_test1 = new AABB(glm::vec2(20,0));

	// draws the plane
	//y
	m_plane = new Plane({ 0, 1 }, 50);
	//x
	m_plane1 = new Plane({ 1, 0 }, 60);
	//y2
	m_plane2 = new Plane({ 0, -1 }, 50);
	//x2
	m_plane3 = new Plane({ -1, 0 }, 60);
	
	m_physicsScene->addActor(m_plane);
	m_physicsScene->addActor(m_plane1);
	m_physicsScene->addActor(m_plane2);
	m_physicsScene->addActor(m_plane3);
	m_physicsScene->addActor(m_test);
	m_physicsScene->addActor(m_test1);

	// adds actor "ball" to the physics scene
//	m_physicsScene->addActor(ball1);
//	m_physicsScene->addActor(ball2);
//	m_physicsScene->addActor(ball3);
//	m_physicsScene->addActor(ball4);
//	m_physicsScene->addActor(ball5);
	//m_physicsScene->addActor(ball6);

	// adds force to ball1 
	ball1->applyForceToActor(ball1, glm::vec2(10000, 10000 * 10));
	m_test->applyForceToActor(ball1, glm::vec2(1000, 0 * 10));


	return true;
}

void PhysicsEngineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;

	delete m_physicsScene;
}

void PhysicsEngineApp::update(float deltaTime) {

	aie::Gizmos::clear();
	// input example
	aie::Input* input = aie::Input::getInstance();

	m_physicsScene->update(deltaTime); 
	m_physicsScene->updateGizmos();

	//const PhysicsScene::collisiondata(collision) ;

	//m_physicsScene->CheckForCollision(object1, object2, );

	
	////Moves a ball across the screen
	static float xPos = 0;
	xPos += 1.0f * deltaTime;


	//	aie::Gizmos::add2DCircle(glm::vec2(0, -20),10.0f, 32, glm::vec4(1, 0, 0, 1));


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	
		quit();
}


void PhysicsEngineApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();


	static float aspectRatio = (float)getWindowWidth() / getWindowHeight(); 

	///DRAWS BREAKOUT 

	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
   	-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	//m_physicsScene->CheckForCollision(object1,object2, );
	
	// draw your stuff here!
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	//m_sphere->makeGizmo();
	m_physicsScene->updateGizmos();
	
	m_2dRenderer->end();
}