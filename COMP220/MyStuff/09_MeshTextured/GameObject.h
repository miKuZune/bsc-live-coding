#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
class GameObject
{
public:
	GameObject();
	~GameObject();

	void loadMeshesFromFile(const std::string& filename);
	void loadDiffuseTextureFromFile(const std::string& filename);
	void loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void preRender();
	void render();
	void update();
	void destroy();

	//Getters and setters

	//Set gameobject position
	void setPosition(const glm::vec3& Position)
	{
		position = Position;
	}
	//Get the position
	const glm::vec3& getPosition()
	{
		return position;
	}
	//Set the scale of the object
	void setScale(const glm::vec3& Scale)
	{
		scale = Scale;
	}
	//Get object scale.
	const glm::vec3& getScale()
	{
		return scale;
	}
	//Set rotation of object
	void setRotation(const glm::vec3& Rotation)
	{
		rotation = Rotation;
	}
	//Get the rotatoin of the object.
	const glm::vec3& getRotation()
	{
		return rotation;
	}
	//Get the model matrix of the model.
	const glm::mat4& getModelMatrix()
	{
		return modelMatrix;
	}
	//Get the diffuseMap of the object.
	const GLuint getDiffuseMap()
	{
		return diffuseMap;
	}

	void setAmbientMaterialColour(const glm::vec4& colour)
	{
		ambientMaterialColour = colour;
	}
	
	const glm::vec4& getAmbientMaterialColour()
	{
		return ambientMaterialColour;
	}

	void setDiffuseMaterialColour(const glm::vec4& colour)
	{
		diffuseMaterialColour = colour;
	}

	const glm::vec4& getDiffuseMaterialColour()
	{
		return diffuseMaterialColour;
	}

	void setSpecularMaterialColour(const glm::vec4& colour)
	{
		SpecularMaterialColour = colour;
	}

	const glm::vec4& getSpecularMaterialColour()
	{
		return SpecularMaterialColour;
	}

	void setSpecularPower(float power)
	{
		specularPower = power;
	}

	const float getSpecularPower()
	{
		return specularPower;
	}

	const GLuint getShaderProgramID()
	{
		return ShaderProgramID;
	}
private:
	//The mesh that is shown
	std::vector<Mesh*> meshes;
	//Model texture pack :^)
	GLuint diffuseMap;
	//Transform
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 modelMatrix;

	//Material
	glm::vec4 ambientMaterialColour;
	glm::vec4 diffuseMaterialColour;
	glm::vec4 SpecularMaterialColour;
	float specularPower;

	GLuint ShaderProgramID;
};
