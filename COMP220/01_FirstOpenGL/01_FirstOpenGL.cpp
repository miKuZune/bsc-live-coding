// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "01_FirstOpenGL.h"

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		MessageBoxA(nullptr, "SDL_Init failed", SDL_GetError(), MB_OK | MB_ICONERROR);
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		MessageBoxA(nullptr, "SDL_CreateWindow failed", SDL_GetError(), MB_OK | MB_ICONERROR);
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr)
	{
		MessageBoxA(nullptr, "SDL_CreateRenderer failed", SDL_GetError(), MB_OK | MB_ICONERROR);
		return 1;
	}

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
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
