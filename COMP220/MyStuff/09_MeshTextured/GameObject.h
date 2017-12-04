#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void setPos(const glm::vec3& position)
	{
		m_Position = position;
	};

	glm::vec3& getPos()
	{
		return m_Position;
	};

	void setScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	};

	glm::vec3& getScale() 
	{
		return m_Scale;
	};

	glm::mat4& getModelMatrix() 
	{
		return modelMatrix;
	};

	GLuint getSharderProgramID()
	{
		return shaderProgramID;
	};


	void loadMesh(const std::string& filename);
	void loadDiffuseMap(const std::string& filename);
	void loadShaderProgram(const std::string & vertextShaderFilename, const std::string& fragmentShaderFilename);

	void update();
	void render();
	void preRender();

	void destroy();


	//set specularPower with (float)
	//get specular power float method to return specularpower

	//get/set for rotation and scale.
private:
	std::vector<Mesh*> m_Meshes;
	GLuint m_DiffuseMapID;
	
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Rot;
	glm::mat4 modelMatrix;

	//add material colours. 
	// glm::vec4 ambientMaterialClour;
	// glm::vec4 diffuseMaterialColour;
	// glm::vec4 specularMaterialColour;
	// float SpecularPower;

	GLuint shaderProgramID;
};