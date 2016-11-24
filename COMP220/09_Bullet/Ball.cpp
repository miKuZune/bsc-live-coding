#include "stdafx.h"
#include "Ball.h"

Ball::Ball(const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
	: position(initialPosition), velocity(initialVelocity)
{

}

void Ball::tick(float deltaTime, float floorY)
{
	// Update position
	position += deltaTime * velocity;

	// Apply collision response
	if (position.y - 1.0f < floorY && velocity.y < 0) // I'm lazy :(
	{
		velocity.y *= -1.0f;
	}

	// Apply gravity
	glm::vec3 acceleration(0, -9.81f, 0);

	// Update velocity
	velocity += deltaTime * acceleration;
}
