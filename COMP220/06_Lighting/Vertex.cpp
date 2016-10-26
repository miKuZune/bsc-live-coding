#include "stdafx.h"
#include "Vertex.h"

Vertex::Vertex(const glm::vec3& position, const glm::vec3& colour, const glm::vec2& textureCoord)
	: m_position(position), m_colour(colour), m_textureCoord(textureCoord)
{
}
