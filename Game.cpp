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

void Game::render(int map[10][10], int xGridSize, int yGridSize)
{
	//Sets background colour for clear command
	SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
	SDL_RenderClear(renderer);

	//Setup Actual draw colour
	SDL_SetRenderDrawColor(renderer, 242, 0, 0, 255);#

	//Draws 2D Map
	drawMap(map, xGridSize, yGridSize);

	SDL_RenderPresent(renderer);
}

void Game::clean()

{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

void Game::drawMap(int map[10][10], int xGridSize, int yGridSize)

{
	int xMapPos, yMapPos, xPos = 0, yPos = 0, xPosReal, yPosReal, cell;
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	while (yPos < yGridSize)
		
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		cell = map[yPos][xPos];
		if (cell == 1)
		{
			if (xPos == 0){ xPosReal = 0; }
			else { xPosReal = ((xPos + 1) * 800 / xGridSize) - (800 / yGridSize);}
			if (yPos == 0) { yPosReal = 0; }
			else{ yPosReal = ((yPos + 1)*600 /yGridSize) - (600/yGridSize);}

			SDL_Rect wall = { xPosReal, yPosReal, 800/xGridSize, 600/yGridSize };
			SDL_RenderDrawRect(renderer, &wall);
		}
		xPos++;
		if (xPos > xGridSize - 1) 
		{
			xPos = 0;
			yPos++;
		}

	}
}