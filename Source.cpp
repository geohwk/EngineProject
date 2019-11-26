#include "Game.h"
#define xGridSize 20
#define yGridSize 20
#define FOV 90
#define xResolution 800
#define yResolution 800
#define planeDistance 50
using namespace std;
Game *game = nullptr;
const float fovRads = FOV * (M_PI / 180);
const int perspectivePlane = 2 * tan(fovRads / 2)*planeDistance;
int map[xGridSize][yGridSize] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 3,
								  3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 3,
								  3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
								  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int main(int argc, char * argv[])
{
	//Keep track of the current frame
	int frame = 0;

	//Whether or not to cap the frame rate
	bool cap = true;

	//The frame rate regulator
	
	const int FPS = 100;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	
	game->init("Map View", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xResolution,yResolution, false);
	const float fovRads = FOV * (M_PI / 180);
	const int perspectivePlane = 2 * tan(fovRads/2)*planeDistance;


	while(game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents(map);
		game->update(map);
		game->render(map, xGridSize, yGridSize);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	};

	game->clean();
	return 0;
}


