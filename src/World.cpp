﻿/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  https://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///-----includes_start-----
#include "World.h"
#include <stdio.h>
#include "DebugRenderer.h"
#include "model_loading/Mesh.h"


/// This is a Hello World program for running a basic Bullet physics simulation

void World::Update(float deltaTime)
{
#ifdef  __DEBUG__
	dynamicsWorld->debugDrawWorld();
	debugDrawer->RenderDebug();

#endif


	dynamicsWorld->stepSimulation(1.f / 60.f, 5); //set it to 5! for the PI
	////print positions of all objects
	//for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//{
	//	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//	if (body && body->getMotionState())
	//	{
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		trans = obj->getWorldTransform();
	//	}
	//	printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()),
	//	       float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//}
}

///-----stepsimulation_end-----

//cleanup in the reverse order of creation/initialization


uint World::AddARigidbody(const btVector3& startinPos)
{
	//create a dynamic rigidbody

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	btCollisionShape* colShape = new btBoxShape(btVector3(1, 1, 1));
	collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(1.0f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(startinPos);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);
	return ID++;
}

uint World::AddAModelRigidbody(const btVector3& startingPos, const std::vector<Mesh>& meshes, float scale)
{
	//btVector3 modelMin(FLT_MAX, FLT_MAX, FLT_MAX); // Initialize to positive infinity
	//btVector3 modelMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);


	////used as starting point for the model
	//btVector3 boxCenter = (modelMax + modelMin) * 0.5f;

	//btVector3 boxHalfExtents = (modelMax - modelMin) * 0.5f;

	//btBoxShape* colShape = new btBoxShape(boxHalfExtents);

	btConvexHullShape* colShape = new btConvexHullShape();

	for (const auto& mesh : meshes)
	{
		for (const auto& vertex : mesh.vertices)
		{
			btVector3 vertexVec(vertex.Position.x, vertex.Position.y, vertex.Position.z);


			colShape->addPoint(vertexVec);
		}
	}

	colShape->setLocalScaling(btVector3(scale, scale, scale));


	collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(startingPos);


	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);
	return ID++;
}

btVector3 World::GetRigidBodyPosition(const uint ID) const
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[ID];
	btRigidBody* body = btRigidBody::upcast(obj);
	btTransform trans;
	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
	}
	else
	{
		trans = obj->getWorldTransform();
	}
	return trans.getOrigin();
}

World::World()
{
	///-----includes_end-----
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	///This thing is callback for collision detection
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(
		dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	debugDrawer = new DebugRenderer();

	// Create your custom debug drawer
	dynamicsWorld->setDebugDrawer(debugDrawer);


	///-----initialization_end-----
	/////keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!


	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(10.), btScalar(50.)));

	collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -20, 0));

	btScalar mass(0.);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(body);
	ID++;
}

World::~World()
{
	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}
