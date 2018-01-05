#include "Particle.h"

Particle::Particle()
{

}

Particle::~Particle()
{
}

void Particle::Initialise()
{
	particle->setPosition(startingPosition);
	particle->setScale(glm::vec3(0.005f, 0.005f, 0.0f));
}

void Particle::SetPositionWithVel()
{
	newPosition = particle->getPosition();
	newPosition += velocity;
	particle->setPosition(newPosition);
}

void Particle::ResetCheck()
{
	if (timeAlive >= timeToReset)
	{
		particle->setPosition(startingPosition);
	}
}
