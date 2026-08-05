#include "Game.h"

const Uint32 FPS = 60;
const Uint32 desiredFrameTime = 1000 / FPS;
Uint32 previousFrameStart = SDL_GetTicks();

int main()
{
	Game* game = new Game();
	game->init();
	while (game->running())
	{
		Uint32 frameStart = SDL_GetTicks();
		float deltaTime = static_cast<float>(frameStart - previousFrameStart) / 1000;
		previousFrameStart = frameStart;
		game->handleEvents();
		game->update(deltaTime);
		game->render();
		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (desiredFrameTime > frameTime)
		{
			SDL_Delay(desiredFrameTime - frameTime);
		}
	}
	game->clean();
	game->quit();
}
