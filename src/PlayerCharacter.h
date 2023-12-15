﻿#pragma once


#include "BulletWrapper.h"

#include "md2_loader/Md2.h"


#include "Timer.h"
#include "model_loading/SkinnedMesh.h"
#include "physics/GameObject.h"
#include "physics/GameValues.h"


class KeyboardManager;

//inspired from Abhishek's presentation
class PlayerCharacter
{
public:
	btTransform SetPositionTransform(const btVector3& startingPosition);
	PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition);
	~PlayerCharacter();
	void CheckForFall();
	void Draw();
	//void SetBoneTransform(uint Index, const Matrix4f& Transform);
	glm::mat4 GetModelMatrix() const;
	void InterpolateFrames(float deltaTime);
	void HandleInput(float deltaTime);
	void MoveCharacter(float deltaTime);
	void Update(float deltaTime);

private:
	//md2model::Md2 model = md2model::Md2("assets/excalibur/tris.md2", "assets/excalibur/alphaone.png");
	btPairCachingGhostObject* playerCharacterGhost = nullptr;
	btKinematicCharacterController* characterController;
	SkinnedMesh* playerModel = nullptr;
	Shader* shader = nullptr;
	float dirX = 0;
	btVector3 dir = btVector3(0, 0, 0);
	float speed = 20.0f;
	float jumpSpeed = 20.0f;
	int displayIndex = 32;
	Timer timer;
#ifdef _WINDOWS
	md2model::Md2 player = md2model::Md2("assets\\excalibur\\tris.md2", "assets\\excalibur\\alphaone.png");
#else
    md2model::Md2 player = md2model::Md2("assets/excalibur/tris.md2", "assets/excalibur/alphaone.png");
#endif

	//GLuint m_boneLocation[MAX_BONES];
	const int startFrame = 0;
	const int endFrame = 197;
	int renderFrame = startFrame;
	// Rendering loop
	float interpolation = 0.0f;
	int bufferIndex = 0;
	GameObject gameObject = GameObject(GameObject::Mesh);
	glm::vec3 position;
	btTransform originalTransform;
	KeyboardManager* inputManager = nullptr;

	float minX = -5.0f; // Replace with your desired minimum X position
	float maxX = 5.0f; // Replace with your desired maximum X position
};