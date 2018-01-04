#include "GameObject.h"

GameObject::GameObject()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	modelMatrix = glm::mat4(1.0f);

	diffuseMap = 0;

	ambientMaterialColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	diffuseMaterialColour = glm::vec4(0.6f,0.6f,0.6f,1.0f);
	SpecularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	specularPower = 25.0f;

	ShaderProgramID = 0;
}

GameObject::~GameObject()
{
}

void GameObject::loadMeshesFromFile(const std::string & filename)
{
	loadMeshFromFile(filename, meshes);

}

void GameObject::loadDiffuseTextureFromFile(const std::string & filename)
{
	diffuseMap = loadTextureFromFile(filename);
}
void GameObject::loadShaders(const std::string & vertexShaderFilename, const std::string & fragmentShaderFilename)
{
	ShaderProgramID = LoadShaders(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
}
void GameObject::preRender()
{
	glUseProgram(ShaderProgramID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	GLint modelMatrixLocation = glGetUniformLocation(ShaderProgramID, "modelMatrix");
	GLint textureLocation = glGetUniformLocation(ShaderProgramID, "baseTexture");

	GLint ambientMaterialColourLocation = glGetUniformLocation(ShaderProgramID, "ambientMaterialColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(ShaderProgramID, "diffuseMaterialColour");
	GLint specularMaterialColourLocation = glGetUniformLocation(ShaderProgramID, "specularMaterialColour");
	GLint specularPowerLocation = glGetUniformLocation(ShaderProgramID, "specularPower");

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	glUniform1i(textureLocation, 0);

	glUniform1fv(ambientMaterialColourLocation, 1, value_ptr(ambientMaterialColour));
	glUniform1fv(diffuseMaterialColourLocation, 1, value_ptr(diffuseMaterialColour));
	glUniform1fv(specularMaterialColourLocation, 1, value_ptr(SpecularMaterialColour));
	glUniform1f(specularPowerLocation, specularPower);
}
void GameObject::render()
{
	for (Mesh *pMesh : meshes)
	{
		pMesh->render();
	}
}
void GameObject::update()
{
	glm::mat4 translationMatrix = glm::translate(position);
	glm::mat4 scaleMatrix = glm::scale(scale);
	glm:: mat4 rotationMatrix = glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))*glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}
void GameObject::destroy()
{
	glDeleteTextures(1, &diffuseMap);
	glDeleteProgram(ShaderProgramID);

	auto destroyable = meshes.begin();
	delete (*destroyable);
	destroyable = meshes.erase(destroyable);
}
