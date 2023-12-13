﻿#pragma once
#include <array>
#include <vector>


#include "Tile.h"
constexpr unsigned int TILES_PER_CHUNK = 18;


class Chunk
{
public:
	~Chunk();
	void LoadTile(size_t index, const char* path, glm::vec3 pos);
	Chunk();

	void Draw();
	void Update(float deltaTime);
	void SetPosition(glm::vec3 pos);

	void SetOffset(glm::vec3 pos);

	glm::vec3 GetPosition();

private:
	std::array<Tile, TILES_PER_CHUNK> tiles;
	std::vector<size_t> activeTiles;
	glm::vec3 offset = glm::vec3(0.0f);
	glm::vec3 position = glm::vec3(0.0f);
	Shader* modelShader = nullptr;
};
