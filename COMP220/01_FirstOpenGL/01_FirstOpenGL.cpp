// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "01_FirstOpenGL.h"

// NOTE: this code is intended to illustrate usage of OpenGL.
// It is NOT intended to illustrate good coding style or naming conventions!
// Code has been copied and pasted from tutorials which use different conventions.
// In your own projects, make sure you rename identifiers to follow a consistent style.

void showErrorMessage(const char* message, const char* title)
{
	// Note: this is specific to Windows, and would need redefining to work on Mac or Linux
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

bool compileShader(GLuint shaderId, const std::string& shaderFileName)
{
	// Read the source code from the file
	std::string shaderSource;
	std::ifstream sourceStream(shaderFileName, std::ios::in);
	if (sourceStream.is_open())
	{
		std::stringstream buffer;
		buffer << sourceStream.rdbuf();
		shaderSource = buffer.str();
		sourceStream.close();
	}
	else
	{
		showErrorMessage(shaderFileName.c_str(), "File not found");
		return false;
	}

	// Compile the shader
	const char* sourcePointer = shaderSource.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	// Check the results of compilation
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1)
	{
		// Display the compilation log
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, errorMessage.data());
		showErrorMessage(errorMessage.data(), shaderFileName.c_str());
	}

	return (result != GL_FALSE);
}

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {

	// Create the shaders
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShaderId, vertex_file_path);

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShaderId, fragment_file_path);

	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Check the program
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, errorMessage.data());
		showErrorMessage(errorMessage.data(), "glLinkProgram error");
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		showErrorMessage("SDL_Init failed", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		showErrorMessage("SDL_CreateWindow failed", SDL_GetError());
		return 1;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		showErrorMessage("SDL_GL_CreateContext failed", SDL_GetError());
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		showErrorMessage("glewInit failed", ":(");
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint programID = loadShaders("vertex.glsl", "fragment.glsl");

	GLuint myColourLocation = glGetUniformLocation(programID, "myColour");
	GLuint timeLocation = glGetUniformLocation(programID, "time");

	float red = 1, green = 0, blue = 0;

	bool running = true;
	while (running)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_r:
					red = 1; green = 0; blue = 0;
					break;

				case SDLK_y:
					red = 1; green = 1; blue = 0;
					break;
				}
			}
		}

		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);

		glUniform3f(myColourLocation, red, green, blue);
		glUniform1f(timeLocation, SDL_GetTicks() / 1000.0f);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
