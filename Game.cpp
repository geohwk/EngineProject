#include "Game.h"
#include <string>
#define xGrid 20
#define yGrid 20
#define xResolution 800
#define yResolution 600
#define radians (5*M_PI)/6
#define FOV 80
#define ScanRes 0.5 //1/0.1
#define WallHeight 25
#define RotationMultiplier 1

using namespace std;

Game::Game()
{
}


Game::~Game()
{
}

int colour;
float scannedViewAngle[FOV*2];
char scannedViewColour[FOV*2];
int scannedViewX[FOV*2];
int scannedViewY[FOV*2];
int viewPointCount = 0;


const int FRAMES_PER_SECOND = 60;
int posMemory[5] = { 100, 100, 100, 80, 90 };
int distanceMultiplier = 3;
double xPosPlayer = posMemory[0], yPosPlayer = posMemory[2], xPlayerView = posMemory[1], yPlayerView = posMemory[3], bearing = posMemory[4], r = 20;
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
		viewWindow = SDL_CreateWindow("Perspective View", xpos, ypos, width, height, flags);
		if (viewWindow)
		{
			std::cout << "Perspective view created!" << std::endl;
		}

		mapWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (mapWindow) 
		{
			std::cout << "Map view created!" << std::endl;
		}
		rendererView = SDL_CreateRenderer(viewWindow, -1, 0);
		if (rendererView)
		{
			std::cout << "View Renderer created!" << std::endl;
		}

		rendererMap = SDL_CreateRenderer(mapWindow, -1, 0);
		if (rendererMap)
		{
			std::cout << "Map Renderer created!" << std::endl;
		}
		isRunning = true;
	}
	else
	{
		std::cout << "SDL initialisation failed!" << std::endl;
		isRunning = false;
	}


	Loading_Surf = SDL_LoadBMP("bricks.bmp");
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	Brick_Tx = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);

	Loading_Surf = SDL_LoadBMP("cobble.bmp");
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	Cobble_Tx = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);

	Loading_Surf = SDL_LoadBMP("sky.bmp");
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	Sky_Tx = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);


	SDL_FreeSurface(Loading_Surf);

}

void Game::handleEvents(int map[xGrid][yGrid])
{
	bearingRads = bearing * (M_PI / 180);
	SDL_Event event;
	bool keyDown = false;
	string currentKey;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	SDL_PumpEvents();
	if (keystate[SDL_SCANCODE_A])
	{
		bearing = bearing - RotationMultiplier;
		if (bearing == (0 - RotationMultiplier))
		{
			bearing = 360 - RotationMultiplier;
		}
		if (xPosPlayer <= 0)
		{
			xPosPlayer = 0;
		}
	}
	else if(keystate[SDL_SCANCODE_W])
	{
		xPosPlayer = xPosPlayer + sin(bearingRads) * distanceMultiplier;
		yPosPlayer = yPosPlayer - cos(bearingRads) * distanceMultiplier;
		xPlayerView = xPlayerView + sin(bearingRads) * distanceMultiplier;
		yPlayerView = yPlayerView - cos(bearingRads) * distanceMultiplier;
		if (yPosPlayer <= 0)
		{
			yPosPlayer = 0;
		}
		if (collisionCheck(map, xPosPlayer, yPosPlayer) == true)
		{
			xPosPlayer = posMemory[0];
			xPlayerView = posMemory[1];
			yPosPlayer = posMemory[2];
			yPlayerView = posMemory[3];
		}
		posMemory[0] = xPosPlayer;
		posMemory[1] = xPlayerView;
		posMemory[2] = yPosPlayer;
		posMemory[3] = yPlayerView;
	}
	else if(keystate[SDL_SCANCODE_S])
	{
		xPosPlayer = xPosPlayer - sin(bearingRads) * distanceMultiplier;
		yPosPlayer = yPosPlayer + cos(bearingRads) * distanceMultiplier;
		xPlayerView = xPlayerView - sin(bearingRads) * distanceMultiplier;
		yPlayerView = yPlayerView + cos(bearingRads) * distanceMultiplier;
		if (yPosPlayer >= yResolution)
		{
			yPosPlayer = yResolution;
		}
		if (collisionCheck(map, xPosPlayer, yPosPlayer) == true)
		{
			xPosPlayer = posMemory[0];
			xPlayerView = posMemory[1];
			yPosPlayer = posMemory[2];
			yPlayerView = posMemory[3];
		}
		posMemory[0] = xPosPlayer;
		posMemory[1] = xPlayerView;
		posMemory[2] = yPosPlayer;
		posMemory[3] = yPlayerView;
	}
	else if(keystate[SDL_SCANCODE_D])
	{
		bearing = bearing + RotationMultiplier;
		if (bearing == 360)
		{
			bearing = 0;
		}
		if (xPosPlayer >= xResolution)
		{
			xPosPlayer = xResolution;
		}
	}
	bearingRads = bearing * (M_PI / 180);
}

void Game::update(int map[xGrid][yGrid])
{
	SDL_SetRenderDrawColor(rendererMap, 250, 250, 250, 255);
	SDL_RenderClear(rendererMap);

	SDL_SetRenderDrawColor(rendererView, 250, 250, 250, 255);
	SDL_RenderClear(rendererView);
	
	SDL_SetRenderDrawColor(rendererMap, 250, 0, 0, 255);
	scan(map);
	SDL_SetRenderDrawColor(rendererMap, 0, 250, 0, 255);
	view();
}

void Game::scan(int map[xGrid][yGrid])
{
	double leftBearingRads, leftBearing, rightBearing, leftScanX, leftScanY, rightScanX, rightScanY;
	leftBearing = (bearing - FOV / 2);
	if (leftBearing < 0)
	{
		leftBearing = 360 + leftBearing;
	}
	rightBearing = (bearing + FOV / 2);
	if (rightBearing > 360)
	{
		rightBearing = rightBearing - 360;
	}
	int flag = 0;
	double dist = 1;
	
	while (leftBearing != rightBearing)
	{
		flag = 0;
		leftBearingRads = leftBearing * (M_PI / 180);
		while (flag == 0)
		{
			leftScanX = sin(leftBearingRads) * dist;
			leftScanY = cos(leftBearingRads) * dist;
			leftScanX = xPosPlayer + leftScanX;
			leftScanY = yPosPlayer - leftScanY;
			if (collisionCheck(map, leftScanX, leftScanY) == true)
			{
				scannedViewAngle[viewPointCount] = bearing - leftBearing;
				scannedViewColour[viewPointCount] = colour;
				scannedViewX[viewPointCount] = leftScanX;
				scannedViewY[viewPointCount] = leftScanY;
				viewPointCount++;
				SDL_RenderDrawLine(rendererMap, xPosPlayer, yPosPlayer, leftScanX, leftScanY);
				flag = 1;
			}
			else
			{
				dist = dist + 0.5;
			}
		}
		dist = 1;
		leftBearing = leftBearing + ScanRes;
		if (leftBearing > 360)
		{
			leftBearing = ScanRes;
		}
	}
	viewPointCount = 0;
}

bool Game::collisionCheck(int map[xGrid][yGrid], int xPosPlayer, int yPosPlayer)
{
	int recWidth = xResolution / xGrid, recHeight = yResolution / yGrid;
	int cellSizeX, cellSizeY;
	int arrayCellX, arrayCellY;
	//Take current xPos/yPos and work out position on map.
	cellSizeX = xResolution / xGrid;
	cellSizeY = yResolution / yGrid;
	arrayCellX = (xPosPlayer / cellSizeX);
	arrayCellY = (yPosPlayer / cellSizeY);

	if (map[arrayCellY][arrayCellX] == 1) //Point is in a collision block
	{
		colour = 1;
		return true;
	}
	if (map[arrayCellY][arrayCellX] == 2) //Point is in a collision block
	{
		colour = 2;
		return true;
	}
	if (map[arrayCellY][arrayCellX] == 3) //Point is in a collision block
	{
		colour = 3;
		return true;
	}
}

void Game::view()
{
	xPlayerView = sin(bearingRads)*r;
	yPlayerView = cos(bearingRads)*r;
	xPlayerView = xPosPlayer + xPlayerView;
	yPlayerView = yPosPlayer - yPlayerView;
	SDL_RenderDrawLine(rendererMap, xPosPlayer, yPosPlayer, xPlayerView, yPlayerView);
	int textureX = 0;
	int count = 0;
	string desiredTx;
	double d, h, yTop, yBottom, x = 0, theta, m1, m2;
	while (count < (FOV*2))
	{
		d = cos(scannedViewAngle[count] * (M_PI / 180))*sqrt(pow((xPosPlayer - scannedViewX[count]), 2) + pow((yPosPlayer - scannedViewY[count]), 2));
		h = yResolution / d * WallHeight;
		yTop = yResolution / 2 + h / 2;
		yBottom = yResolution / 2 - h / 2;
		SDL_SetRenderDrawColor(rendererView, 0, 250, 0, 255);
		double xStore = x;

		

		SDL_Rect wall = { x, yResolution - yTop, fabs(((xStore + xResolution) / (FOV / ScanRes))) , yTop - yBottom };
		//SDL_RenderFillRect(rendererView, &wall);
		//SDL_RenderCopy(rendererView, Brick_Tx, NULL, NULL);

		
		textureX++;

		/*
		if (textureX > 200)
		{
			textureX = 0;
		}
		*/
		SDL_Rect wallex = { x, 0,fabs(((xStore + xResolution) / (FOV / ScanRes))), 200  };

		if (scannedViewColour[count] == 1)
		{
			SDL_RenderCopy(rendererView, Cobble_Tx, &wallex, &wall);
			//SDL_SetRenderDrawColor(rendererView, 0, 250, 0, 255);
		}
		if (scannedViewColour[count] == 2)
		{
			
			SDL_RenderCopy(rendererView, Brick_Tx, &wallex, &wall);
			//SDL_SetRenderDrawColor(rendererView, 250, 250, 0, 255);
		}
		if (scannedViewColour[count] == 3)
		{
			SDL_RenderCopy(rendererView, Cobble_Tx, &wallex, &wall);
			//desiredTx = "Brick_Tx";
			//SDL_SetRenderDrawColor(rendererView, 0, 250, 250, 255);
		}
		
		//SDL_RenderPresent(rendererView);

		SDL_SetRenderDrawColor(rendererView, 218, 218, 218, 255);
		SDL_Rect ceiling = { x, 0, fabs(((xStore + xResolution) / (FOV / ScanRes))), yResolution - yTop};
		SDL_RenderCopy(rendererView, Sky_Tx, &ceiling, &ceiling);
		//SDL_RenderFillRect(rendererView, &ceiling);
			
		SDL_SetRenderDrawColor(rendererView, 150, 150, 150, 255);
		SDL_Rect floor = { x, yTop, fabs(((xStore + xResolution) / (FOV / ScanRes))), yResolution - yBottom};
		SDL_RenderFillRect(rendererView, &floor);

		x = xStore + xResolution / (FOV/ScanRes);
		count++;
	}





}

void Game::render(int map[xGrid][yGrid], int xGridSize, int yGridSize)
{
	//Sets background colour for clear command
	

	//Setup Actual draw colour
	SDL_SetRenderDrawColor(rendererMap, 242, 0, 0, 255);

	//Draws 2D Map
	drawMap(map, xGridSize, yGridSize);
	SDL_RenderPresent(rendererView);
	SDL_RenderPresent(rendererMap);
}

void Game::clean()

{
	SDL_DestroyWindow(mapWindow);
	SDL_DestroyRenderer(rendererMap);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

void Game::drawMap(int map[xGrid][yGrid], int xGridSize, int yGridSize)

{
	int xMapPos, yMapPos, xPos = 0, yPos = 0, xPosReal, yPosReal, cell, cellXMemory = 0, cellYMemory = 0, originXPos = 1000, originYPos;
	
	SDL_SetRenderDrawColor(rendererMap, 0, 0, 255, 255);
	
	while (yPos < yGridSize)
	{
		SDL_SetRenderDrawColor(rendererMap, 0, 0, 255, 255);
		cell = map[yPos][xPos];
		if ( xPos == 0)
		{
			cellXMemory = 0;
		}
		else
		{
			cellXMemory = map[yPos][xPos - 1];
		}
		if (cell != 0)
		{
			if (cellXMemory != 0)
			{
				//do nothing, we're waiting for the end point of the rectangle
			}
			else
			{
				//Create origin of rectangle as previous cell is not drawn
				originXPos = xPos;
				originYPos = yPos;
			}
			
		}
		if (cell < 1)
		{
			if (originXPos == 1000)
			{
				//No origin has been set therefore we don't draw anything 
			}
			else
			{
				//Origin has been set therefore we need to draw a rectangle from origin X, Y and cellMemory
				if (originXPos == 0) { xPosReal = 0; }
				else { xPosReal = ((originXPos + 1) * 800 / xGridSize) - (800 / yGridSize); }
				if (originYPos == 0) { yPosReal = 0; }
				else { yPosReal = ((originYPos + 1) * 600 / yGridSize) - (600 / yGridSize); }

				SDL_Rect wall = { xPosReal, yPosReal, (800 / xGridSize)*(xPos - originXPos), 600 / yGridSize };
				SDL_RenderDrawRect(rendererMap, &wall);
				originXPos = 1000;
			}
		}
		xPos++;
		if (xPos > xGridSize - 1) 
		{
			if (originXPos == 0) { xPosReal = 0; }
			else { xPosReal = ((originXPos + 1) * 800 / xGridSize) - (800 / yGridSize); }
			if (originYPos == 0) { yPosReal = 0; }
			else { yPosReal = ((originYPos + 1) * 600 / yGridSize) - (600 / yGridSize); }
			
			SDL_Rect wall = { xPosReal, yPosReal, (800 / xGridSize) * (xPos), 600 / yGridSize };
			SDL_RenderDrawRect(rendererMap, &wall);
			originXPos = 1000;
			xPos = 0;
			yPos++;
		}
	}
}