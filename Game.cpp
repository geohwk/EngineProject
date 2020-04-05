#include "Game.h"
#include <string>
#define xGrid 20
#define yGrid 20
#define xResolution 800
#define yResolution 800
#define xResolutionScreen 1200
#define yResolutionScreen 800
#define radians (5*M_PI)/6
#define FOV 90
#define ScanRes 0.5 //1/0.1
#define WallHeight 20
#define RotationMultiplier 4
#define textureDim 180
#define planeDistance 50
#define ScanInc 0.3
#define setDistanceMultiplier 3
#define textureWidth 1200
#define textureHeight 800

using namespace std;

Game::Game()
{
}


Game::~Game()
{
}


const int perspectivePlane = 100;
constexpr int perspectiveWidth = perspectivePlane / ScanRes;

int shoot = 0, walkingFlag = 0;
int colour;
int gunHeight = 10, up = 0;
int wallSide[perspectiveWidth];
float texturePosition[perspectiveWidth];
float scannedViewAngle[perspectiveWidth];
char scannedViewColour[perspectiveWidth];
float scannedViewX[perspectiveWidth];
float scannedViewY[perspectiveWidth]; //make these based on perspective plane size (perspectivePlane/0.5)
float scannedEntityX[perspectiveWidth];
float scannedEntityY[perspectiveWidth];
float scannedEntityColour[perspectiveWidth];

int viewPointCount = 0;

int posMemory[5] = { 100, 100, 100, 80, 90 };
int distanceMultiplier = 3;
double xPosPlayer = posMemory[0], yPosPlayer = posMemory[2], xPlayerView = posMemory[1], yPlayerView = posMemory[3], bearing = posMemory[4], r = 20;
double bearingRads;


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
		viewWindow = SDL_CreateWindow("Perspective View", 50, 50, xResolutionScreen, yResolutionScreen, flags);
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
		SDL_SetRenderDrawBlendMode(rendererView, SDL_BLENDMODE_BLEND);

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


	//Loading_Surf = SDL_LoadBMP("brickTexture.bmp");
	Loading_Surf = SDL_LoadBMP("bricks.bmp");
	
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	Brick_Tx1 = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);//default texture
	Brick_Tx0 = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);//dimmed texture
	SDL_SetTextureColorMod(Brick_Tx0, textureDim, textureDim, textureDim);

	Loading_Surf = SDL_LoadBMP("cobbleTexture.bmp");
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

	Loading_Surf = SDL_LoadBMP("gun.bmp");
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	Gun_Tx = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);

	Loading_Surf = SDL_LoadBMP("gunShot.bmp");
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	GunEx_Tx = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);
	
	Loading_Surf = SDL_LoadBMP("mob.bmp");
	if (Loading_Surf == NULL)
	{
		cout << "Unable to load image" << endl;
	}
	Enemy_Tx = SDL_CreateTextureFromSurface(rendererView, Loading_Surf);
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
	shoot = 0;
	if (keystate[SDL_SCANCODE_LSHIFT])
	{
		distanceMultiplier = setDistanceMultiplier + 3;
	}
	if (!keystate[SDL_SCANCODE_LSHIFT])
	{
		distanceMultiplier = setDistanceMultiplier;
	}
	if (keystate[SDL_SCANCODE_SPACE])
	{
		int randomNum = rand() % 100;
		if (randomNum > 80)
		{
			shoot = 0;
		}
		else
		{
			shoot = 1;
		}
		
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		bearing = bearing - RotationMultiplier;
		if (bearing < 0)
		{
			bearing = 360 - RotationMultiplier;
		}
		if (xPosPlayer <= 0)
		{
			xPosPlayer = 0;
		}

	}
	if (keystate[SDL_SCANCODE_RIGHT])
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
	if (keystate[SDL_SCANCODE_A])
	{
		float sideBearing = bearingRads - (float)M_PI / 2;
		if (sideBearing < 0)sideBearing = 2 * M_PI + sideBearing;
		xPosPlayer = xPosPlayer + sin(sideBearing) * distanceMultiplier;
		yPosPlayer = yPosPlayer - cos(sideBearing) * distanceMultiplier;
		xPlayerView = xPlayerView + sin(sideBearing) * distanceMultiplier;
		yPlayerView = yPlayerView - cos(sideBearing) * distanceMultiplier;

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
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S])
	{
		walkingFlag = 1;
		if (up == 0)
		{
			gunHeight = gunHeight + 10;
			if (gunHeight >= 100)
			{
				up = 1;
			}
		}
		if (up == 1)
		{
			gunHeight = gunHeight - 10;
			if (gunHeight <= 0)
			{
				up = 0;
			}
		}
	}
	if (!(keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S]) && (walkingFlag == 1))
	{
		if (up == 0)
		{
			gunHeight = gunHeight + 10;
			if (gunHeight >= 100)
			{
				up = 1;
			}
		}
		if (up == 1)
		{
			gunHeight = gunHeight - 10;
			if (gunHeight <= 0)
			{
				walkingFlag = 0;
			}
		}
	}
	if (keystate[SDL_SCANCODE_D])
	{
		float sideBearing = bearingRads + (float)M_PI / 2;
		if (sideBearing > 360)sideBearing = sideBearing - 360;
		xPosPlayer = xPosPlayer + sin(sideBearing) * distanceMultiplier;
		yPosPlayer = yPosPlayer - cos(sideBearing) * distanceMultiplier;
		xPlayerView = xPlayerView + sin(sideBearing) * distanceMultiplier;
		yPlayerView = yPlayerView - cos(sideBearing) * distanceMultiplier;

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
	if(keystate[SDL_SCANCODE_W])
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
	if(keystate[SDL_SCANCODE_S])
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
	for (int j = 0; j < perspectiveWidth; j++)
	{
		scannedEntityColour[j] = 0;
	}

}

void Game::scan(int map[xGrid][yGrid])
{
	double roundedX, roundedY;
	double leftBearingRads, leftBearing, rightBearing, leftScanX, leftScanY, rightScanX, rightScanY;
	int side;
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
	double dist = ScanInc;
	double planePosition = perspectivePlane / 2;
	double perspectiveBearingRads = atan((planePosition) / planeDistance);
	double perspectiveBearing = 0;
	
	leftBearingRads = leftBearing * (M_PI / 180);
	while (perspectiveBearing > -45)
	{
		
		flag = 0;
		//leftBearingRads = leftBearing * (M_PI / 180);
		while (flag == 0)
		{
			leftScanX = sin(leftBearingRads) * dist;
			leftScanY = cos(leftBearingRads) * dist;
			leftScanX = xPosPlayer + leftScanX;
			leftScanY = yPosPlayer - leftScanY;
			if (collisionCheck(map, leftScanX, leftScanY) == true)
			{
				float temp = 0;
				roundedX = round(leftScanX);
				roundedY = round(leftScanY);

				//TODO: Work out how to determine when section of texture is on the gridpoint
				if ((int)roundedX % 40 == 0) //Side is facing sideways (along X axis) 
				{
					if (leftBearing < 360 && leftBearing >= 180)
					{
						temp = 40 - fmod(leftScanY, 40); //right facing
					}
					else {
						temp = fmod(leftScanY, 40); //left facing
					}
					side = 0;
				}
				else //Side is facing up (along Y axis)
				{
					if (leftBearing <= 270 && leftBearing >= 90)
					{
						temp = 40 - fmod(leftScanX, 40); //Down facing
					}
					else {
						temp = (fmod(leftScanX, 40)); //Up facing

					}
					side = 1;
				}

				if (((int)roundedX % 40 == 0) && ((int)roundedY % 40 == 0)) {
					side = wallSide[viewPointCount - 1];
				}

				wallSide[viewPointCount] = side;
				texturePosition[viewPointCount] = temp;
				scannedViewAngle[viewPointCount] = bearing - leftBearing;
				scannedViewColour[viewPointCount] = colour;
				scannedViewX[viewPointCount] = leftScanX;
				scannedViewY[viewPointCount] = leftScanY;

				if (scannedEntityColour[viewPointCount] != 4)
				{
					scannedEntityColour[viewPointCount] = 0;
				}
				
				viewPointCount++;
				
				SDL_RenderDrawLine(rendererMap, xPosPlayer, yPosPlayer, leftScanX, leftScanY);
				
				flag = 1;

			}
			else
			{
				dist = dist + ScanInc;
			}
		}
		dist = ScanInc;
		planePosition = planePosition - ScanRes;
		perspectiveBearingRads = atan((planePosition) / planeDistance);
		leftBearingRads = bearingRads - perspectiveBearingRads;
		//leftBearing = leftBearing + ScanRes;
		perspectiveBearing = perspectiveBearingRads * (180 / M_PI);
		leftBearing = leftBearingRads * (180 / M_PI);
		if (leftBearing >= 360)
		{
			leftBearing = leftBearing - 360;
			leftBearingRads = leftBearing * (M_PI / 180);
		}
		if (leftBearing < 0)
		{
			leftBearing = 360 + leftBearing;
			leftBearingRads = leftBearing * (M_PI / 180);
		}
	}
	viewPointCount = 0;
}

bool Game::collisionCheck(int map[xGrid][yGrid], int xPosPlayer, int yPosPlayer)
{
	int recWidth = xResolutionScreen / xGrid, recHeight = yResolutionScreen / yGrid;
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
	if (map[arrayCellY][arrayCellX] == 4) //Point is in entity collision block
	{
		colour = 4;
		if (scannedEntityColour[viewPointCount] != colour)
		{
			scannedEntityColour[viewPointCount] = colour;
			scannedEntityX[viewPointCount] = xPosPlayer;
			scannedEntityY[viewPointCount] = yPosPlayer;
		}
		return false;
	}
}

void Game::view()
{
	xPlayerView = sin(bearingRads)*r;
	yPlayerView = cos(bearingRads)*r;
	xPlayerView = xPosPlayer + xPlayerView;
	yPlayerView = yPosPlayer - yPlayerView;

	SDL_RenderDrawLine(rendererMap, xPosPlayer, yPosPlayer, xPlayerView, yPlayerView);
	int count = 0;
	int storedX = 0;
	string desiredTx;
	double d, dEntity, h, hEntity, yTop, yBottom, yTopEntity, yBottomEntity, x = 0, theta, m1, m2;
	double xStore;
	int width;
	while (count < (perspectivePlane/ScanRes))
	{
		d = cos(scannedViewAngle[count] * (M_PI / 180))*sqrt(pow((xPosPlayer - scannedViewX[count]), 2) + pow((yPosPlayer - scannedViewY[count]), 2));
		dEntity = cos(scannedViewAngle[count] * (M_PI / 180))*sqrt(pow((xPosPlayer - scannedEntityX[count]), 2) + pow((yPosPlayer - scannedEntityY[count]), 2));
		
		h = yResolutionScreen / d * WallHeight;
		hEntity = yResolutionScreen / dEntity * WallHeight;

		yTopEntity = yResolutionScreen / 2 + hEntity / 2;
		yBottomEntity = yResolutionScreen / 2 - hEntity / 2;

		yTop = yResolutionScreen / 2 + h / 2;
		yBottom = yResolutionScreen / 2 - h / 2;
		SDL_SetRenderDrawColor(rendererView, 0, 250, 0, 255);
		xStore = x;
		
		width = fabs((((texturePosition[count + 1] - texturePosition[count])/40 * textureWidth)));
		if (width == 0)
		{
			width = 1;
		}
		SDL_Rect wall = { x, yResolutionScreen - yTop, xResolutionScreen / (perspectivePlane / ScanRes) , yTop - yBottom };
		SDL_Rect wallex = { (texturePosition[count]/40)*textureWidth, 0,width, textureHeight  };
		
		SDL_Rect entityRect = { x, yResolutionScreen - yTopEntity, xResolutionScreen / (perspectivePlane / ScanRes) , yTopEntity - yBottomEntity };

		if (scannedViewColour[count] == 1)
		{
			SDL_RenderCopy(rendererView, Cobble_Tx, &wallex, &wall);
		}
		if (scannedViewColour[count] == 2)
		{
			if (wallSide[count] == 1)
			{
				SDL_RenderCopy(rendererView, Brick_Tx1, &wallex, &wall); //Default Texture
			}
			else
			{
 				SDL_RenderCopy(rendererView, Brick_Tx0, &wallex, &wall); //Dimmed Texture
			}
			
		}
		
		if (scannedViewColour[count] == 3)
		{
			SDL_RenderCopy(rendererView, Cobble_Tx, &wallex, &wall);
		}

		/*
		if (scannedViewColour[count] == 4)
		{
			SDL_RenderCopy(rendererView, Enemy_Tx, &wallex, &wall);
		}
		*/

		SDL_Rect ceilingex = { (texturePosition[count] / 40) * 100, 0,width, 200 };
		
		SDL_SetRenderDrawColor(rendererView, 0, 0, 0, 255);
		SDL_Rect ceiling = { x, 0, fabs(((xStore + xResolutionScreen) / (perspectivePlane / ScanRes))), yResolutionScreen - yTop};
		SDL_RenderCopy(rendererView, Sky_Tx, &ceilingex, &ceiling);
		//SDL_RenderFillRect(rendererView, &ceiling);
			
		SDL_SetRenderDrawColor(rendererView, 100, 150, 150, 255);
		SDL_Rect floor = { x, yTop, fabs(((xStore + xResolutionScreen) / (perspectivePlane / ScanRes))), yResolutionScreen - yBottom};
		SDL_RenderFillRect(rendererView, &floor);
		
		if (scannedEntityColour[count] == 4)
		{
			SDL_RenderCopy(rendererView, Enemy_Tx, &wallex, &entityRect);
		}
		
		
		
		
		
		
		storedX = x;
		x = xStore + (xResolutionScreen / (perspectivePlane/ScanRes));
		count++;
		//SDL_RenderPresent(rendererView);
	}
	SDL_Rect Gun_tx = { 0, 0, xResolutionScreen, yResolutionScreen };
	if (shoot == 0)
	{
		SDL_Rect Gun = { 0, gunHeight, xResolutionScreen, yResolutionScreen };
		SDL_RenderCopy(rendererView, Gun_Tx, &Gun_tx, &Gun);
	}
	else {
		SDL_Rect GunShot = { 0, gunHeight, xResolutionScreen, yResolutionScreen };
		SDL_RenderCopy(rendererView, GunEx_Tx, &Gun_tx, &GunShot);
	}
	
}

void Game::render(int map[xGrid][yGrid], int xGridSize, int yGridSize)
{
	//Sets background colour for clear command
	

	//Setup Actual draw colour
	SDL_SetRenderDrawColor(rendererMap, 0, 100, 100, 255);

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
				else { xPosReal = ((originXPos + 1) * xResolution / xGridSize) - (xResolution / yGridSize); }
				if (originYPos == 0) { yPosReal = 0; }
				else { yPosReal = ((originYPos + 1) * yResolution / yGridSize) - (yResolution / yGridSize); }

				SDL_Rect wall = { xPosReal, yPosReal, (xResolution / xGridSize)*(xPos - originXPos), yResolution / yGridSize };
				SDL_RenderDrawRect(rendererMap, &wall);
				originXPos = 1000;
			}
		}
		xPos++;
		if (xPos > xGridSize - 1) 
		{
			if (originXPos == 0) { xPosReal = 0; }
			else { xPosReal = ((originXPos + 1) * xResolution / xGridSize) - (xResolution / yGridSize); }
			if (originYPos == 0) { yPosReal = 0; }
			else { yPosReal = ((originYPos + 1) * yResolution / yGridSize) - (yResolution / yGridSize); }
			
			SDL_Rect wall = { xPosReal, yPosReal, (xResolution / xGridSize) * (xPos), yResolution / yGridSize };
			SDL_RenderDrawRect(rendererMap, &wall);
			originXPos = 1000;
			xPos = 0;
			yPos++;
		}
	}
}

class PlayerHand
{
public:

	void renderImplement()
	{

	}

};