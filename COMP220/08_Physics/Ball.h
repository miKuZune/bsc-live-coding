#pragma once

class Ball
{
public:
	Ball(const glm::vec3& initialPosition, const glm::vec3& initialVelocity);

	glm::vec3 getPosition() { return position; }

	void tick(float deltaTime, float ohNoINeedToKnowFloorY);

private:
	glm::vec3 position;
	glm::vec3 velocity;
};
