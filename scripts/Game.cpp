#include "Game.h"

void Game::init()
{
	isRunning = true;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
		isRunning = false;
	}

	if (!SDL_CreateWindowAndRenderer("Super Mario Bros", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
	{
		std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
		isRunning = false;
	}
	SDL_SetRenderVSync(renderer, 1);
	camera = new Camera();
	tileMap = new TileMap();
	player = std::make_unique<Player>(keys);
	entities.push_back(std::move(player));
	collisionManager = new CollisionManager(entities);
	camera->init();
	tileMap->init(renderer);
	collisionManager->init();
	for (const auto& entity : entities)
	{
		entity->init(renderer);
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
	for (const auto& entity : entities)
	{
		entity->update(deltaTime, *collisionManager);
		if (entity->getType() == EntityType::PlayerType)
		{
			SDL_FRect playerRect = entity->getRect();
			camera->update(playerRect);
		}
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 92, 148, 252, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	tileMap->render(renderer, camera);
	for (const auto& entity : entities)
	{
		entity->draw(renderer, camera);
	}
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