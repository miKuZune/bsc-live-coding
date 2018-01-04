#include "Particle.h"

Particle::Particle()
{
	ParticleStartUp();
}

Particle::~Particle()
{
}

void Particle::ParticleStartUp()
{
	//sets up the look of the particle
	particle->loadMeshesFromFile("Cube.fbx");
	particle->loadDiffuseTextureFromFile("Cube.png");
	particle->loadShaders("textureVert.glsl", "textureFrag.glsl");
	//set positional stuff
	SetStartPos(glm::vec3(0, 0, 0));
	velocity = glm::vec3(1, 1, 1);
	//Set lifetime stuff
	timeAlive = 0;
}

//Checks if it is time for the particle to be deleted
bool Particle::CheckIfLifeIsOver()
{
	if (timeAlive >= lifeTime)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

//Moves particle by it's velocity, called each frame
void Particle::ParticleMoveInDirection()
{
	glm::vec3 newPos = particle->getPosition() - velocity;
	particle->setPosition(newPos);
}

//Rotates the particle so that it faces the player, called each frame
void Particle::RotateTowardPlayer(glm::vec3 playerPos)
{
}
//Actions to be performed for the particle each frame
void Particle::EachFrame()
{
	ParticleMoveInDirection();
	CheckIfLifeIsOver();
	//RotateTowardPlayer();
}
