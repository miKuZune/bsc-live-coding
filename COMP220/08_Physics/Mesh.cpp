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

	if (m_uvBuffer != 0)
		glDeleteBuffers(1, &m_uvBuffer);

	if (m_normalBuffer != 0)
		glDeleteBuffers(1, &m_normalBuffer);
}

void Mesh::addTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	glm::vec3 normal = glm::cross(v2.m_position - v1.m_position, v3.m_position - v1.m_position);
	normal = glm::normalize(normal);

	v1.m_normal = normal;
	v2.m_normal = normal;
	v3.m_normal = normal;

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
}

void Mesh::addVertex(const Vertex& vertex)
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("Cannot add vertices after createBuffers() has been called");
	}

	m_vertexPositions.push_back(vertex.m_position);
	m_vertexColours.push_back(vertex.m_colour);
	m_vertexUVs.push_back(vertex.m_textureCoord);
	m_vertexNormals.push_back(vertex.m_normal);
}

Vertex Mesh::createSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour)
{
	glm::vec3 unitPos(
		cos(latitude) * cos(longitude),
		sin(latitude),
		cos(latitude) * sin(longitude));

	glm::vec2 textureCoords(
		-longitude / glm::radians(360.0f),
		latitude / glm::radians(180.0f) + 0.5f);

	return Vertex(radius * unitPos,
		colour,
		textureCoords);
}

void Mesh::addSphere(float radius, int quality, const glm::vec3& colour)
{
	float angleStep = glm::radians(90.0f) / quality;

	std::vector<Vertex> lastRingPoints, ringPoints;
	
	// Top cap
	float latitude = angleStep * (quality - 1);
	for (int i = 0; i <= quality * 4; i++)
	{
		float longitude = i * angleStep;
		ringPoints.push_back(createSphereVertex(radius, longitude, latitude, colour));
		if (ringPoints.size() > 1)
		{
			Vertex pole = createSphereVertex(radius, longitude - 0.5f*angleStep, glm::radians(90.0f), colour);
			addTriangle(pole, ringPoints[i], ringPoints[i - 1]);
		}
	}

	// Rings
	for (int j = quality - 2; j > -quality; j--)
	{
		lastRingPoints.clear();
		std::swap(lastRingPoints, ringPoints);

		float latitude = angleStep * j;
		for (int i = 0; i <= quality * 4; i++)
		{
			float longitude = i * angleStep;
			ringPoints.push_back(createSphereVertex(radius, longitude, latitude, colour));
			if (ringPoints.size() > 1)
			{
				addTriangle(lastRingPoints[i], ringPoints[i], ringPoints[i - 1]);
				addTriangle(lastRingPoints[i - 1], lastRingPoints[i], ringPoints[i - 1]);
			}
		}
	}

	// Bottom cap
	for (int i = 1; i < ringPoints.size(); i++)
	{
		Vertex pole = createSphereVertex(radius, (i - 0.5f)*angleStep, glm::radians(-90.0f), colour);
		addTriangle(pole, ringPoints[i - 1], ringPoints[i]);
	}
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

	// Create and fill the texture coordinate buffer
	glGenBuffers(1, &m_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexUVs.size() * sizeof(glm::vec2), m_vertexUVs.data(), GL_STATIC_DRAW);

	// Create and fill the normal buffer
	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexNormals.size() * sizeof(glm::vec3), m_vertexNormals.data(), GL_STATIC_DRAW);
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

	// Bind the texture coordinate buffer to vertex attribute 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the normal buffer to vertex attribute 3
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
