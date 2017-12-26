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

	std::vector<Mesh*> meshes;
	loadMeshFromFile("smaug.fbx", meshes);
	GLuint textureID = loadTextureFromFile("skin-t.tga");

	vec3 trianglePosition = vec3(0.0f,-5.0f,10.0f);
	vec3 triangleScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 triangleRotation = vec3(140.0f, 3.10f, 0.0f);

	
	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(triangleScale);
	mat4 rotationMatrix= rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));

	mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;

	vec3 cameraPosition = vec3(0.0f, 5.0f, -10.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);


	GLuint programID = LoadShaders("textureVert.glsl", "textureFrag.glsl");

	GLint fragColourLocation=glGetUniformLocation(programID, "fragColour");
	if (fragColourLocation < 0)
	{
		printf("Unable to find %s uniform\n", "fragColour");
	}

	static const GLfloat fragColour[] = { 0.0f,1.0f,0.0f,1.0f };

	GLint currentTimeLocation= glGetUniformLocation(programID, "time");
	if (currentTimeLocation < 0)
	{
		printf("Unable to find %s uniform\n", "time");
	}

	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");

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

	//Model pos
	vec3 newModelRot = triangleRotation;

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


		//newCamUp.z += 0.1f;
		//cout << newCamUp.z << "\n";
		//Update camera position and target.
		viewMatrix = lookAt(newCamPos, newCamTarget, newCamUp);
		rotationMatrix = rotate(newModelRot.x, vec3(1.0f, 0.0f, 0.0f))*rotate(newModelRot.y, vec3(0.0f, 1.0f, 0.0f))*rotate(newModelRot.z, vec3(0.0f, 0.0f, 1.0f));
		mat4 modelMatrix = translationMatrix * rotationMatrix*scaleMatrix;


		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUseProgram(programID);

		glUniform4fv(fragColourLocation, 1, fragColour);
		glUniform1f(currentTimeLocation, (float)(currentTicks)/1000.0f);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniform1i(textureLocation, 0);

		for (Mesh *pMesh : meshes)
		{
			pMesh->render();
		}
		SDL_GL_SwapWindow(window);

		lastTicks = currentTicks;
	}

	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	meshes.clear();
	glDeleteTextures(1, &textureID);
	glDeleteProgram(programID);

	SDL_GL_DeleteContext(GL_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}