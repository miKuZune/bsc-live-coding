#pragma once

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& colour);
	void addSquare(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& colour);

	void createBuffers();
	void draw();

private:
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0;
};
