#pragma once
#include "glm\vec3.hpp"
#include "GameObject.h"
#include "glm/gtc/type_ptr.hpp"
class Particle
{
public:
	Particle();
	~Particle();
	
	
	void ParticleStartUp();
	bool CheckIfLifeIsOver();
	void ParticleMoveInDirection();
	void RotateTowardPlayer(glm::vec3 playerPos);

	void EachFrame();

	//Set the starting pos of the particle
	void SetStartPos(glm::vec3 startPos)
	{
		particle->setPosition(startPos);
	}
	//set the life time of the particles
	void setLifeTime(float timeToDelete)
	{
		lifeTime = timeToDelete;
	}
	//Do gameobject stuff
	void ParticleGameObjectUpdate()
	{
		particle->update();
	}
	void ParticleGameObjectPreRender()
	{
		particle->preRender();
	}
	void ParticleGameObjectRender()
	{
		particle->render();
	}
	//Destroy this
	void Destroy()
	{
		delete(particle);
	}
private:
	
	GameObject * particle;

	glm::vec3 velocity;

	float lifeTime;
	float timeAlive;
};