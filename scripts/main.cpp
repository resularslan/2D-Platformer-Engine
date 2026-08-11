#include "Game.h"
#include <cmath>

Uint64 previousFrameStart;
float maxDeltaTime = 0.05f;


int main()
{
	Game* game = new Game();
	game->init();
	previousFrameStart = SDL_GetPerformanceCounter();
	while (game->running())
	{
		Uint64 frameStart = SDL_GetPerformanceCounter();
		float deltaTime = std::fmin(maxDeltaTime, (float) (frameStart - previousFrameStart) / SDL_GetPerformanceFrequency());
		previousFrameStart = frameStart;
		game->handleEvents();
		game->update(deltaTime);
		game->render();
		/*game->delay(frameStart);*/
	}
	game->clean();
	game->quit();
}
