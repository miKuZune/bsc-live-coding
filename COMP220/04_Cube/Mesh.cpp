#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if (m_positionBuffer != 0)
		glDeleteBuffers(1, &m_positionBuffer);

	if (m_colourBuffer != 0)
		glDeleteBuffers(1, &m_colourBuffer);
}

void Mesh::addTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& colour)
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("Cannot add triangles after createBuffers() has been called");
	}

	// Add the vertex positions
	m_vertexPositions.push_back(p1);
	m_vertexPositions.push_back(p2);
	m_vertexPositions.push_back(p3);
	
	// Make all three vertices the same colour
	for (int i = 0; i < 3; i++)
		m_vertexColours.push_back(colour);
}

void Mesh::addSquare(const glm::vec3& a, const glm::vec3& b,
	const glm::vec3& c, const glm::vec3& d, const glm::vec3& colour)
{
	addTriangle(a, b, d, colour);
	addTriangle(d, b, c, colour);
}

void Mesh::createBuffers()
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("createBuffers() has already been called");
	}

	// Create and fill the position buffer
	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexPositions.size() * sizeof(glm::vec3), m_vertexPositions.data(), GL_STATIC_DRAW);

	// Create and fill the colour buffer
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexColours.size() * sizeof(glm::vec3), m_vertexColours.data(), GL_STATIC_DRAW);
}

void Mesh::draw()
{
	if (m_positionBuffer == 0)
	{
		throw std::exception("createBuffers() must be called before draw()");
	}

	// Bind the position buffer to vertex attribute 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the colour buffer to vertex attribute 1
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
