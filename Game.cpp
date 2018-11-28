#include "Game.h"

#define xResolution 800
#define yResolution 600


Game::Game()
{
}


Game::~Game()
{
}

int xPosPlayer = 100, yPosPlayer = 100;

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
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				yPosPlayer--;
				if (yPosPlayer <= 0)
				{
					yPosPlayer = 0;
				}
				break;
			case SDLK_a:
				xPosPlayer--;
				if (xPosPlayer <= 0)
				{
					xPosPlayer = 0;
				}
				break;
			case SDLK_d:
				xPosPlayer++;
				if (xPosPlayer >= xResolution)
				{
					xPosPlayer = xResolution;
				}
				break;
			case SDLK_s:
				yPosPlayer++;
				if (yPosPlayer >= yResolution)
				{
					yPosPlayer = yResolution;
				}
				break;
			default:
				break;
			}
		default:
			break;
	}
}

void Game::update()
{
	SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
	SDL_RenderDrawLine(renderer, xPosPlayer, yPosPlayer, xPosPlayer, yPosPlayer - 20);
	
	
	std::cout << xPosPlayer << yPosPlayer << std::endl;
	
}

void Game::render(int map[10][10], int xGridSize, int yGridSize)
{
	//Sets background colour for clear command
	

	//Setup Actual draw colour
	SDL_SetRenderDrawColor(renderer, 242, 0, 0, 255);

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