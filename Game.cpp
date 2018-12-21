#include "Game.h"

#define xResolution 800
#define yResolution 600
#define radians (5*M_PI)/6


Game::Game()
{
}


Game::~Game()
{
}

int xPosPlayer = 100, yPosPlayer = 100, xPlayerView = xPosPlayer, yPlayerView = yPosPlayer - 20, bearing = 90, r = 20;
double bearingRads, reverseBearing, reverseBearingRads;
bool interset, viewComplete;

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
	bearingRads = bearing * (M_PI / 180);
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
				yPlayerView = yPosPlayer - (cos(bearingRads)*(r + 1));
				xPlayerView = xPosPlayer + (sin(bearingRads)*(r + 1));
				reverseBearing = bearing + 180;
				reverseBearing = reverseBearing * (M_PI / 180);
				xPosPlayer = xPlayerView - sin(reverseBearingRads)*r;
				yPosPlayer = yPlayerView + cos(reverseBearingRads)*r;
				
				if (yPosPlayer <= 0)
				{
					yPosPlayer = 0;
				}
				break;
			case SDLK_a:
				bearing = bearing - 5;
				if (bearing == -5)
				{
					bearing = 355;
				}
				if (xPosPlayer <= 0)
				{
					xPosPlayer = 0;
					//xPosPlayer = 0;
				}
				break;
			case SDLK_d:
				bearing = bearing + 5;
				if (bearing == 360)
				{
					bearing = 0;
				}
				
				if (xPosPlayer >= xResolution)
				{
					xPosPlayer = xResolution;
				}
				break;
			case SDLK_s:
				yPlayerView = yPosPlayer + (cos(bearingRads)*(r - 1));
				xPlayerView = xPosPlayer - (sin(bearingRads)*(r - 1));
				reverseBearing = bearing + 180;
				reverseBearing = reverseBearing * (M_PI / 180);
				xPosPlayer = xPlayerView + sin(reverseBearing)*r;
				yPosPlayer = yPlayerView - cos(reverseBearing)*r;
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
	bearingRads = bearing * (M_PI / 180);
	std::cout << bearing << std::endl;
}

void Game::update()
{
	SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
	SDL_RenderClear(renderer);
	
	
	SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
	collisionCheck(xPosPlayer, yPosPlayer);
	view();
	
	
	
	std::cout << bearing << std::endl;
	
}

void Game::movement()
{

}

void Game::collisionCheck(int xPosPlayer, int yPosPlayer)
{
	SDL_IntersectRectAndLine( )
}

void Game::view()
{
	xPlayerView = sin(bearingRads)*r;
	yPlayerView = cos(bearingRads)*r;
	xPlayerView = xPosPlayer + xPlayerView;
	yPlayerView = yPosPlayer - yPlayerView;
	SDL_RenderDrawLine(renderer, xPosPlayer, yPosPlayer, xPlayerView, yPlayerView);
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
	int xMapPos, yMapPos, xPos = 0, yPos = 0, xPosReal, yPosReal, cell, cellMemory, orginXPos = 1000, originYPos;
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	while (yPos < yGridSize)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		cell = map[yPos][xPos];
		if ((yPos || xPos) == 0)
		{
			cellMemory == 0;
		}
		else if(xPos == 0)
		{
			cellMemory = 0;
		}
		else
		{
			cellMemory = map[yPos][xPos - 1];
		}
		if (cell == 1)
		{
			if (cellMemory == 1)
			{
				//do nothing, we're waiting for the end point of the rectangle
			}
			else
			{
				//Create origin of rectangle as previous cell is not drawn
				orginXPos = xPos;
				originYPos = yPos;
			}
			
		}
		if (cell == 0)
		{
			if (orginXPos == 1000)
			{
				//No origin has been set therefore we don't draw anything 
			}
			else
			{
				//Origin has been set therefore we need to draw a rectangle from origin X, Y and cellMemory
				if (xPos == 0) { xPosReal = 0; }
				else { xPosReal = ((xPos + 1) * 800 / xGridSize) - (800 / yGridSize); }
				if (yPos == 0) { yPosReal = 0; }
				else { yPosReal = ((yPos + 1) * 600 / yGridSize) - (600 / yGridSize); }

				SDL_Rect wall = { xPosReal, yPosReal, 800 / xGridSize*(xPos + 1), 600 / yGridSize*(yPos + 1) };
				SDL_RenderDrawRect(renderer, &wall);
				
			}
		}
		xPos++;
		if (xPos > xGridSize - 1) 
		{
			xPos = 0;
			yPos++;
		}
	}
}