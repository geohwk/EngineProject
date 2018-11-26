#include "Game.h"



Game::Game()
{
}


Game::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL initialization succeeded!" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) 
		{
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer created!" << std::endl;
		}
		isRunning = true;
	}
	else
	{
		std::cout << "SDL initialisation failed!" << std::endl;
		isRunning = false;
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) 
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;

	}
}

void Game::update()
{

}

void Game::render()
{

}

void Game::clean()
{

}

