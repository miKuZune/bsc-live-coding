//main.cpp - defines the entry point of the application

#include "main.h"
#include <iostream>

using namespace std;

int main(int argc, char* args[])
{
#pragma region WindowInitalisation

	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return 1;
	}

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	int windowWidth = 800, windowHeight = 600;

	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//lets ask for a 3.2 core profile version of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext GL_Context = SDL_GL_CreateContext(window);
	if (GL_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL GL Create Context failed", NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(glewError), "GLEW Init Failed", NULL);
	}

#pragma endregion

#pragma region CameraAndMatricies

	//Camera
	vec3 cameraPosition = vec3(0.0f, 5.0f, -10.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);
	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);

	static const GLfloat fragColour[] = { 0.0f,1.0f,0.0f,1.0f };

	//Movement variables
	vec3 newCamPos = cameraPosition;
	vec3 newCamTarget = cameraTarget;
	vec3 newCamUp = cameraUp;

	float mouseX = 0, mouseY = 0;

	float moveSpeed = 0.35f;
#pragma endregion

#pragma region MainGameObjects

	//List to store and deal with all game objects during runtime
	//This list will be used to render all of the gameobjects in the scene.
	std::vector<GameObject*> GameObjectList;
	//Gameobjects
	GameObject * dragon = new GameObject;
	dragon->setPosition(vec3(0.0f, -10.0f, 10.0f));
	dragon->setRotation(vec3(139.5f, 3.10f, 0.0f));
	dragon->setScale(vec3(0.5f, 0.5f, 0.5f));
	dragon->loadMeshesFromFile("Dragon.fbx");
	dragon->loadDiffuseTextureFromFile("Dragon (2).png");
	dragon->loadShaders("textureVert.glsl", "textureFrag.glsl");

	GameObjectList.push_back(dragon);

	GameObject * chest = new GameObject;
	chest->loadMeshesFromFile("ChestCartoon.fbx");
	chest->loadDiffuseTextureFromFile("chest_rare.png");
	chest->loadShaders("textureVert.glsl", "textureFrag.glsl");
	chest->setPosition(vec3(15.0f, -5.0f, 10.0f));
	chest->setRotation(vec3(140.0f, 3.1f, 90.0f));
	bool chestPickedUp = false;
	bool chestOnFire = false;
	float timeOnFire = 0;
	GameObjectList.push_back(chest);

	GameObject * barrel = new GameObject;
	barrel->loadMeshesFromFile("barrel.fbx");
	barrel->loadDiffuseTextureFromFile("barrel.png");
	barrel->loadShaders("textureVert.glsl", "textureFrag.glsl");
	barrel->setPosition(vec3(20.0f, -5.0f, 10.0f));
	barrel->setRotation(vec3(0,0, 0));
	barrel->setScale(vec3(0.1f, 0.1f, 0.1f));
	float barrelMovement = -0.1f;
	float amountBarelMoved = 0;
	GameObjectList.push_back(barrel);

#pragma endregion

#pragma region ParticleGameObjects

	std :: vector<GameObject*> ParticleList;
	//Creating GameObjects to use as particle effects
	const int particleAmount = 50;
	GameObject * particleArry[particleAmount];
	for (int i = 0; i < particleAmount; i++)
	{
		particleArry[i] = new GameObject;
		particleArry[i]->loadMeshesFromFile("Cube.fbx");
		particleArry[i]->loadDiffuseTextureFromFile("Cube.png");
		particleArry[i]->loadShaders("textureVert.glsl", "textureFrag.glsl");
		particleArry[i]->setPosition(vec3(0, 0, 0));
		particleArry[i]->setScale(vec3(0.005f, 0.005f, 0.0f));
		GameObjectList.push_back(particleArry[i]);
		ParticleList.push_back(particleArry[i]);
	}

	//Creating GOs for the chest fire particles
	std::vector<GameObject *> chestParticles;
	const int chestParCount = particleAmount / 5;
	GameObject * chestPartArry[chestParCount];
	
	for (int i = 0; i < chestParCount; i++)
	{
		chestPartArry[i] = new GameObject;
		chestPartArry[i]->loadMeshesFromFile("Cube.fbx");
		chestPartArry[i]->loadDiffuseTextureFromFile("Cube.png");
		chestPartArry[i]->loadShaders("textureVert.glsl", "textureFrag.glsl");
		chestPartArry[i]->setPosition(chest->getPosition());
		chestPartArry[i]->setScale(vec3(0.005f, 0.005f, 0.0f));
		GameObjectList.push_back(chestPartArry[i]);
		chestParticles.push_back(chestPartArry[i]);
	}
#pragma endregion

#pragma region LoopStuff

	//Loop handling
	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();

	//whether or not the game is running
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
#pragma endregion

#pragma region ParticleInitalisation
	//Variables for moving the particles during runtime
	vec3 newPos;

	vec3 velocities[particleAmount];
	//Variables used for randomising the direction of particle movement
	float x = 0, y = 0, z = 0;
	srand(time(NULL));

	//Dragon breath particles
	float timesToLive[particleAmount];
	float timesAlive[particleAmount];
	float temp;
	//Loop to deal with giving all particles random vairables.
	for (int i = 0; i < particleAmount; i++)
	{
		//Randomising the velocity of the particles.
		x = (rand() % 200) ;
		y = (rand() % 100 + -100) ;
		z = (rand() % 100 + -100);
		velocities[i] = vec3((x - 100)/500,y/500,z/100);
		//Randomising the lifetime of the particles.
		temp = rand() % 100;
		timesToLive[i] = temp / 100;
		timesAlive[i] = 0;
	}


	//Chest particles setting random variables
	vec3 chestVelocities[chestParCount];
	float chestParTimeToLive[chestParCount];
	float chestParTimeAlive[chestParCount];
	for (int i = 0; i < chestParCount; i++)
	{
		chestVelocities[i] = vec3(0, 0, 0);
		chestParTimeToLive[i] = 0;
		chestParTimeAlive[i] = 0;
	}

#pragma endregion

#pragma region MainLoop



	while (running)
	{
		

#pragma region EventHandler
		
		vec3 rayEndPos;
		//Calculate and normalise the forward direction of the camera for use throughout the loop.
		vec3 camForward = newCamPos - newCamTarget;

		//Normalisation of camera forward.
		float camForLength = (camForward.x * camForward.x) + (camForward.y * camForward.y) + (camForward.z * camForward.z);
		camForLength = sqrt(camForLength);
		camForward = camForward / camForLength;

		camForward = -camForward;


		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{

			float xDelta = 0, yDelta = 0;

			
			vec3 left = vec3(0, 0, 1);
			vec3 camLeft;
			//Cross product (put into method)
			camLeft.x = (camForward.y * newCamUp.z) - (camForward.z * newCamUp.y);
			camLeft.y = (camForward.z * newCamUp.x) - (camForward.x * newCamUp.z);
			camLeft.z = (camForward.x * newCamUp.y) - (camForward.y * newCamUp.x);

			camLeft = camLeft * 2.5f;

			
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;
				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					newCamPos += camForward * moveSpeed;
					newCamTarget += camForward * moveSpeed;
					break;
				case SDLK_s:
					newCamPos -= camForward * moveSpeed;
					newCamTarget -= camForward * moveSpeed;
					break;
				case SDLK_a:
					newCamPos -= camLeft * (moveSpeed);
					newCamTarget -= camLeft * (moveSpeed);
					break;
				case SDLK_d:
					newCamPos += camLeft * (moveSpeed);
					newCamTarget += camLeft * (moveSpeed);
					break;
				case SDLK_e:
					
					vec3 chestPos = chest->getPosition();
					vec3 chestPosUpperLimit = chestPos + chest->getScale();
					rayEndPos = newCamPos + (camForward * 20.0f);

					chestPickedUp = !chestPickedUp;
					break;
				}
			case SDL_MOUSEMOTION:
				mouseY = ev.motion.yrel;
				mouseX = ev.motion.xrel;

				xDelta = ev.motion.xrel;
				yDelta = ev.motion.yrel;

				newCamTarget.x += xDelta / -50;
				newCamTarget.y += yDelta / -50;
			}
		}
#pragma endregion

		
#pragma region rendering

		//Camera updated every frame
		viewMatrix = lookAt(newCamPos, newCamTarget, newCamUp);


		//GameObject rendering
		for (GameObject* currObj : GameObjectList)
		{
			currObj->update();
		}

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		GLuint currProgID = dragon->getShaderProgramID();

		for (GameObject* currObj : GameObjectList)
		{
			currObj->preRender();
			GLint viewMatrixLocation = glGetUniformLocation(currProgID, "viewMatrix");
			GLint projectionMatrixLocation = glGetUniformLocation(currProgID, "projectionMatrix");

			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));

			currObj->render();
		}


#pragma endregion


#pragma region Collision
		vec3 checkObjUpperLimit;
		vec3 checkObj;

		checkObj = chest->getPosition();
		checkObjUpperLimit = checkObj + chest->getScale();


		//Checks if the chest has collided with any of the particles in the particle list.
		for (GameObject* currParticle : ParticleList)
		{
			//Get the current particle position and scale to build a check area 
			vec3 currPartPos = currParticle->getPosition();
			vec3 currPartUpperLim = currParticle->getScale() + currPartPos;

			//Check if the particles and the chest have touched
			if (currPartPos.x >= checkObj.x && currPartPos.y >= checkObj.y && currPartPos.z >= checkObj.z)
			{
				if (currPartUpperLim.x <= checkObjUpperLimit.x && currPartUpperLim.y <= checkObjUpperLimit.y && currPartUpperLim.z <= checkObjUpperLimit.z)
				{
					chestOnFire = true;
					timeOnFire = 0;
					//Setup particles when chest is set on fire
					for (int i = 0; i < chestParCount; i++)
					{
						//Randomising the velocity of the particles.
						x = (rand() % 200);
						y = (rand() % 200);
						z = (rand() % 100);
						chestVelocities[i] = vec3((x - 100) / 500, y / 500, z / 100);
						//Randomising the lifetime of the particles.
						temp = rand() % 100;
						chestParTimeToLive[i] = temp / 100;
						chestParTimeAlive[i] = 0;
						cout << chestVelocities[i].x << "\n";
					}
				}
			}
		}

#pragma endregion

#pragma region NonEventBasedGameplay
		//Allows the barrel to move back and forth
		vec3 barrelNewPos = barrel->getPosition();

		barrelNewPos.x += barrelMovement;
		barrel->setPosition(barrelNewPos);

		amountBarelMoved += barrelMovement;
		if (amountBarelMoved >= 5.0f || amountBarelMoved <= -5.0f)
		{
			//Resets the barrel after it has moved a certain distance
			amountBarelMoved = 0;
			barrelMovement = -barrelMovement;
		}

		//Checks if the chest has been picked up by the player and holds the chest slightly infront of the player.
		if (chestPickedUp)
		{
			rayEndPos = newCamPos + (camForward * 12.5f);
			chest->setPosition(rayEndPos);
		}

		//Checks if the chest is not on fire, and deals with the chest fire particles accordingly.
		if (!chestOnFire)
		{
			for (GameObject * currPart : chestParticles)
			{
				vec3 holdPos = chest->getPosition();
				holdPos.z -= 2;
				currPart->setPosition(holdPos);
			}
		}
#pragma endregion


#pragma region GeneralStuff
		//Delta time and window displaying
		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;
		SDL_GL_SwapWindow(window);

		lastTicks = currentTicks;
#pragma endregion

#pragma region ParticleRuntimeHandling
		//Loop to deal particles during runtime.
		//(Placed here to make use of the delta time variable)
		for (int i = 0; i < particleAmount; i++)
		{
			//Moves the current particle by the appropriate velocity of the same position.
			newPos = particleArry[i]->getPosition();
			newPos = newPos + velocities[i];

			particleArry[i]->setPosition(newPos);

			if (timesAlive[i] > timesToLive[i])
			{
				particleArry[i]->setPosition(vec3(0, 0, 0));

				temp = rand() % 100;
				x = rand() % 200;
				y = (rand() % 100 + -100);
				z = (rand() % 100 + -100);
				timesToLive[i] = temp / 100;
				velocities[i] = vec3((x - 100) / 500, y / 500, z / 100);
				timesAlive[i] = 0;
			}
			timesAlive[i] += deltaTime;
		}

		//Chest particles
		//Does the same as the normal particles but only if the chest has been set on fire.
		if (chestOnFire) {
			for (int i = 0; i < chestParCount; i++)
			{
				
				newPos = chestPartArry[i]->getPosition();
				newPos = newPos + chestVelocities[i];

				chestPartArry[i]->setPosition(newPos);

				timesAlive[i] += deltaTime;

				if (timesAlive[i] > timesToLive[i])
				{
					chestPartArry[i]->setPosition(chest->getPosition());

					//Randomising direction of travel
					x = (rand() % 1000);
					y = (rand() % 200);
					z = (rand() % 1000);
					chestVelocities[i] = vec3((x - 500) / 1000, y / 400, (z - 500) / 1000);
					//Randomising the lifetime of the particles.
					temp = rand() % 100;
					chestParTimeToLive[i] = temp / 100;
					chestParTimeAlive[i] = 0;
				}
			}
			
			//Tracks how long the chest has been on fire and puts out the fire after X amount of time.
			timeOnFire += deltaTime;
			
			if (timeOnFire >= 12.5f)
			{
				chestOnFire = false;
				timeOnFire = 0;
			}
		}
#pragma endregion

	}

#pragma endregion

#pragma region EndOfApplicationCleanup
	//Deleting gameObjects
	auto gameObjectIter = GameObjectList.begin();
	while (gameObjectIter != GameObjectList.end())
	{
		if ((*gameObjectIter))
		{
			(*gameObjectIter)->destroy();
			delete (*gameObjectIter);
			GameObjectList.erase(gameObjectIter);
		}
	}


	//Deleting particles


	//Remember to delete all the particles 

	SDL_GL_DeleteContext(GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
#pragma endregion

}