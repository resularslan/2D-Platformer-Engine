#include "Game.h"

void Game::init()
{
	isRunning = true;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
		isRunning = false;
	}

	if (!SDL_CreateWindowAndRenderer("Super Mario Bros", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
	{
		std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
		isRunning = false;
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				isRunning = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				keys[event.key.scancode] = true;
				break;
			case SDL_EVENT_KEY_UP:
				keys[event.key.scancode] = false;
				break;
		}
	}
}

void Game::update(float deltaTime)
{
	return;
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 92, 148, 252, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Game::quit()
{
	SDL_Quit();
}

bool Game::running()
{
	return isRunning;
}