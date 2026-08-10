#include "Game.h"
#include <cmath>

const int FPS = 60;
const Uint64 desiredFrameTime = SDL_GetPerformanceFrequency() / FPS;
Uint64 previousFrameStart = SDL_GetPerformanceCounter();
float maxDeltaTime = 0.1f;
int safetyThreshold = 2;

int main()
{
	Game* game = new Game();
	game->init();
	while (game->running())
	{
		Uint64 frameStart = SDL_GetPerformanceCounter();
		float deltaTime = std::fmin(maxDeltaTime, (float) (frameStart - previousFrameStart) / SDL_GetPerformanceFrequency());
		previousFrameStart = frameStart;
		game->handleEvents();
		game->update(deltaTime);
		game->render();
		Uint64 frameTick = SDL_GetPerformanceCounter() - frameStart;
		if (desiredFrameTime > frameTick)
		{
			Uint32 remainingTime = ((desiredFrameTime - frameTick) * 1000) / SDL_GetPerformanceFrequency();
			if (remainingTime > safetyThreshold)
			{
				SDL_Delay(remainingTime - safetyThreshold);
			}
		}
		while ((SDL_GetPerformanceCounter() - frameStart) < desiredFrameTime)
		{

		}
	}
	game->clean();
	game->quit();
}
