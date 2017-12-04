#include "GameObject.h"

GameObject::GameObject()
{
	m_Meshes.clear();

	m_DiffuseMapID = 0;

	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f); // all 1
	m_Rot = glm::vec3(0.0f, 0.0f, 0.0f);
	modelMatrix = glm::mat4(1.0f);
}
void GameObject::destroy()
{
	glDeleteTextures(1, &m_DiffuseMapID);

	//Theres meant to be a loop here :/
}
void GameObject::loadDiffuseMap(const std::string & filename)
{
	m_DiffuseMapID = loadTextureFromFile(filename);
}
void GameObject::loadShaderProgram(const std::string & vertextShaderFilename, const std::string& fragmentShaderFilename)
{
	GLuint programID = LoadShaders(vertextShaderFilename.c_str(), fragmentShaderFilename.c_str());
}
void GameObject::update()
{
	glm::mat4 translationMatrix = glm::translate(m_Position);
	glm::mat4 scaleMatrix = glm::scale(m_Scale);
	glm::mat4 rotationMatrix = glm::rotate(m_Rot.x, glm::vec3(1.0f, 0.0f, 0.0f))*glm::rotate(m_Rot.y, glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(m_Rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;
}
void GameObject::render()
{
	for (Mesh *pMesh : m_Meshes)
	{
		pMesh->render();
	}
}
void GameObject::preRender()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMapID);

	glUseProgram(shaderProgramID);

	

	GLint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	GLint textureLocation = glGetUniformLocation(shaderProgramID, "baseTexture");
	/*
	glUniform4fv(fragColourLocation, 1, fragColour);
	glUniform1f(specularPowerLocation, (float)(currentTicks) / 1000.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
	glUniform1i(textureLocation, 0);*/

	
}
#pragma region materials.
	//Add materials here

	//
#pragma endregion

