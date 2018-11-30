#include "Game.h"
#define xGridSize 10
#define yGridSize 10

Game *game = nullptr;

int main(int argc, char * argv[])
{
	int map[xGridSize][yGridSize] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
									  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
									  1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
					                  1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
					                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					                  1, 0, 0, 1, 0, 1, 0, 0, 0, 1,
					                  1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
					                  1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
					                  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					                  1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	game = new Game();

	game->init("George Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while(game->running())
	{
		game->handleEvents();
		game->update();
		game->render(map, xGridSize, yGridSize);
	};

	game->clean();
	return 0;
}


