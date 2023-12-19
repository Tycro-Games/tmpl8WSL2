﻿#include "StaticMesh.h"

#include "Camera.h"
#include "Camera.h"
#include "game.h"
#include "template.h"
#include "physics/DebugRenderer.h"


StaticMesh::StaticMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices,
                       std::vector<StaticMesh::MeshTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	//setupMesh();
}

void StaticMesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw StaticMesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//    // ambient
//  
//    highp vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//    
//     // diffuse 
//    highp vec3 norm = normalize(Normal);
//    highp vec3 lightDir = normalize(lightPos - FragPos);
//    highp float diff = max(dot(norm, lightDir), 0.0);
//    highp vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
//
//     
//     // specular
//    highp vec3 viewDir = normalize(viewPos - FragPos);
//    highp vec3 reflectDir = reflect(-lightDir, norm);  
//    highp float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    highp vec3 specular = light.specular * (spec * material.specular);   
void StaticMesh::BakeLighting(glm::vec3& worldPos)
{
	//get data from texture
	if (textures.size() > 0)
	{
		string path = "assets/tiled/castle/" + textures[0].path;
		Surface surface(path.c_str());

		for (auto& vertex : vertices)
		{
			// ambient


			glm::vec3 ambient = glm::vec3(0.2f);

			// diffuse
			glm::vec3 norm = glm::normalize(vertex.Normal);
			glm::vec3 lightDir = glm::normalize(Game::GetLightPos() - vertex.Position);
			float diff = max(glm::dot(norm, lightDir), 0.0f);
			float distance = length(Game::GetLightPos() - (vertex.Position + worldPos));
			float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
			glm::vec3 diffuse = glm::vec3(1.0f) * diff;
			//// specular
			//glm::vec3 viewDir = glm::normalize(viewPos - vertex.Position);
			//glm::vec3 reflectDir = reflect(-lightDir, norm);
			//float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
			//	glm::vec3 specular = light.specular * (spec * material.specular);
			vertex.Color = ambient + diffuse;
		}
	}
	//for (auto& vertex : vertices)
	//{
	//	// ambient

	//	glm::vec3 ambient = light.ambient * glm::vec3(texture(material.diffuse, TexCoords));

	//	// diffuse 
	//	glm::vec3 norm = glm::normalize(Normal);
	//	glm::vec3 lightDir = normalize(lightPos - FragPos);
	//	float diff = max(dot(norm, lightDir), 0.0);
	//	glm::vec3 diffuse = light.diffuse * diff * glm::vec3(texture(material.diffuse, TexCoords));


	//	// specular
	//	glm::vec3 viewDir = normalize(viewPos - FragPos);
	//	glm::vec3 reflectDir = reflect(-lightDir, norm);
	//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//	glm::vec3 specular = light.specular * (spec * material.specular);
	//	vertex.Color = glm::glm::vec3(1.0f);
	//}
}

void StaticMesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	             &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex color 
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	glBindVertexArray(0);
}
