#include "SDL.h"
#include <iostream>
#include <stdio.h>
class Game
{
public:
	//Constructor
	Game();
	//Deconstructor
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	void render(int map[10][10], int xGridSize, int yGridSize);
	void clean();

	bool running() 
	{
		return isRunning;
	}
private:
	void drawMap(int map[10][10], int xGridSize, int yGridSize);
	void view();
	void movement();
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
};

