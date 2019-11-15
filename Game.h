#include "SDL.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#define xGrid 20
#define yGrid 20
class Game
{
public:
	//Constructor
	Game();
	//Deconstructor
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void scan(int map[xGrid][yGrid]);
	bool collisionCheck(int map[xGrid][yGrid], int xPosPlayer, int yPosPlayer);
	void handleEvents(int map[xGrid][yGrid]);
	void update(int map[xGrid][yGrid]);
	void render(int map[xGrid][yGrid], int xGridSize, int yGridSize);
	void clean();

	bool running() 
	{
		return isRunning;
	}
private:
	void drawMap(int map[xGrid][yGrid], int xGridSize, int yGridSize);
	void view();
	void movement();
	bool isRunning;
	SDL_Window *mapWindow;
	SDL_Window *viewWindow;
	SDL_Renderer *rendererMap;
	SDL_Renderer *rendererView;
	SDL_Surface* Loading_Surf;
	SDL_Texture* Brick_Tx;
	SDL_Texture* Cobble_Tx;
	SDL_Texture* Sky_Tx;
};

