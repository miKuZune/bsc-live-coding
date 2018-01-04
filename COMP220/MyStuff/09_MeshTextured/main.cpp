//main.cpp - defines the entry point of the application

#include "main.h"
#include <iostream>

using namespace std;

int main(int argc, char* args[])
{
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

	//Camera
	vec3 cameraPosition = vec3(0.0f, 5.0f, -10.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);
	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);

	static const GLfloat fragColour[] = { 0.0f,1.0f,0.0f,1.0f };

	std::vector<GameObject*> GameObjectList;
	//Gameobjects
	GameObject * dragon = new GameObject;
	dragon->setPosition(vec3(0.0f, -10.0f, 10.0f));
	dragon->setRotation(vec3(139.5f, 3.10f, 0.0f));
	dragon->setScale(vec3(0.5f, 0.5f, 0.5f));
	dragon->loadMeshesFromFile("Dragon.fbx");
	dragon->loadDiffuseTextureFromFile("Dragon (2).png");
	dragon->loadShaders("textureVert.glsl", "textureFrag.glsl");

	//GameObjectList.push_back(dragon);

	GameObject * chest = new GameObject;
	chest->loadMeshesFromFile("ChestCartoon.fbx");
	chest->loadDiffuseTextureFromFile("chest_rare.png");
	chest->loadShaders("textureVert.glsl", "textureFrag.glsl");
	chest->setPosition(vec3(15.0f, -5.0f, 10.0f));
	chest->setRotation(vec3(140.0f, 3.1f, 90.0f));

	//GameObjectList.push_back(chest);

	GameObject * barrel = new GameObject;
	barrel->loadMeshesFromFile("barrel.fbx");
	barrel->loadDiffuseTextureFromFile("barrel.png");
	barrel->loadShaders("textureVert.glsl", "textureFrag.glsl");
	barrel->setPosition(vec3(15.0f, -5.0f, 10.0f));
	barrel->setRotation(vec3(0,0, 0));
	barrel->setScale(vec3(0.1f, 0.1f, 0.1f));

	//GameObjectList.push_back(barrel);


	
	//Particle effects
	//Particle list
	//Particle objects
	GameObject * firstParticle = new GameObject;
	firstParticle->loadMeshesFromFile("Cube.fbx");
	firstParticle->loadDiffuseTextureFromFile("Cube.png");
	firstParticle->loadShaders("textureVert.glsl", "textureFrag.glsl");
	firstParticle->setPosition(vec3(0, 0, 0));
	firstParticle->setScale(vec3(0.005f, 0.005f, 0.0f));

	GameObjectList.push_back(firstParticle);

#pragma region KillIt


	/*
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");
	*/

	//Particle system
	/*int maxParticles = 254;

	static const GLfloat vertexBufferData[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		0.5f,0.5f,0.0f,
	};
	GLuint billboardVertexBuffer;
	glGenBuffers(1, &billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	GLuint particlePosBuffer;
	glGenBuffers(1, &particlePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlePosBuffer);

	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	GLuint particleColourBuff;
	glGenBuffers(1, &particleColourBuff);
	glBindBuffer(GL_ARRAY_BUFFER, particleColourBuff);

	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	*/

#pragma endregion

	//other stuff
	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();


	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;

	//Movement vari
	vec3 newCamPos = cameraPosition;
	vec3 newCamTarget = cameraTarget;
	vec3 newCamUp = cameraUp;

	float mouseX = 0, mouseY = 0;

	float moveSpeed = 0.35f;

	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{

			float xDelta = 0, yDelta = 0;

			vec3 camForward = newCamPos - newCamTarget;

			//Normalisation (put into method)
			float camForLength = (camForward.x * camForward.x) + (camForward.y * camForward.y) + (camForward.z * camForward.z);
			camForLength = sqrt(camForLength);
			camForward = camForward / camForLength;

			//camForward needs inversing.
			camForward = -camForward;
			
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
					newCamTarget -= camLeft * (moveSpeed );
					break;
				case SDLK_d:
					newCamPos += camLeft * (moveSpeed);
					newCamTarget += camLeft * (moveSpeed);
					break;
				case SDLK_r:
					newCamPos = vec3(0.0f, 5.0f, -10.0f);
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


		//Camera updater
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


		
		//Particle effect rendering


		/*glBindBuffer(GL_ARRAY_BUFFER, particlePosBuffer);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0, maxParticles * sizeof(GLfloat) * 4, )*/
		
		
		//send shader values

		//glActiveTexture(GL_TEXTURE0);
		/*
		glUniform4fv(fragColourLocation, 1, fragColour);
		glUniform1f(currentTimeLocation, (float)(currentTicks)/1000.0f);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniform1i(textureLocation, 0);
		*/

		//EndOfLoop stuff to do
		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		SDL_GL_SwapWindow(window);

		lastTicks = currentTicks;
	}

	//Deleting stuff

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
}