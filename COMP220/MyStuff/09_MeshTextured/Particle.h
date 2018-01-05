#pragma once
#include "glm\vec3.hpp"
#include "GameObject.h"
class Particle
{
public:
	Particle();
	~Particle();

	void Initialise();
	void SetPositionWithVel();
	void ResetCheck();



#pragma region gettersAndSetters

	glm::vec3 GetVelocity()
	{
		return velocity;
	}
	void SetVelocity(glm::vec3 newVel)
	{
		velocity = newVel;
	}
	glm::vec3 GetNewPos()
	{
		return newPosition;
	}
	glm::vec3 GetStartPos()
	{
		return startingPosition;
	}
	void SetStartPos(glm::vec3 newStartPos)
	{
		startingPosition = newStartPos;
	}
	float GetTimeAlive()
	{
		return timeAlive;
	}
	void SetTimeAlive(float newTimeAlive)
	{
		timeAlive = newTimeAlive;
	}
	float GetTimeToReset()
	{
		return timeToReset;
	}
	void SetTimeToReset(float newTime)
	{
		timeToReset = newTime;
	}

	GameObject GetGameObj()
	{
		return * particle;
	}
	void setGameObj(GameObject newGO)
	{
		particle = &newGO;
	}

#pragma endregion


private:
	//Stores the particle gameobject
	GameObject * particle;
	//The velocity used to move the particle and the variable to calculate the new position.
	glm::vec3 velocity;
	glm::vec3 newPosition;
	//Starting position used when reseting the particle.
	glm::vec3 startingPosition;
	//Used to calculate time to check if the particle needs to be reset
	float timeAlive;
	float timeToReset;

	
};