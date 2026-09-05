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
	insectInfos[0] = { 44 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[1] = { 80 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE  };
	insectInfos[2] = { 102 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE  };
	insectInfos[3] = { 105 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE  };
	insectInfos[4] = { 160 * CELL_SIZE, WINDOW_HEIGHT - 22 * CELL_SIZE };
	insectInfos[5] = { 164 * CELL_SIZE, WINDOW_HEIGHT - 22 * CELL_SIZE };
	insectInfos[6] = { 194 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[7] = { 197 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[8] = { 228 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[9] = { 231 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[10] = { 248 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[11] = { 251 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[12] = { 256 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[13] = { 259 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[14] = { 348 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	insectInfos[15] = { 351 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	turtleInfo = { 214 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	growMushroomInfo = { 20 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE };
	camera = new Camera();
	tileMap = new TileMap();
	for (int i = 0; i < 16; i++)
	{
		insects[i] = std::make_unique<Insect>(insectInfos[i].xPos, insectInfos[i].yPos, renderer, *camera, entityCount++);
		entities.push_back(std::move(insects[i]));
	}
	turtle = std::make_unique<Turtle>(turtleInfo.xPos, turtleInfo.yPos, renderer, *camera, entityCount++);
	entities.push_back(std::move(turtle));
	growMushroom = std::make_unique<GrowMushroom>(growMushroomInfo.xPos, growMushroomInfo.yPos, renderer, *camera, entityCount++);
	entities.push_back(std::move(growMushroom));
	player = std::make_unique<Player>(keys, CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE, renderer, *camera, entityCount++);
	entities.push_back(std::move(player));
	collisionManager = new CollisionManager(entities);
	camera->init();
	tileMap->init(renderer);
	collisionManager->init();
	for (const auto& entity : entities)
	{
		entity->init();
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
		SDL_FRect entityRect = entity->getRect();
		if (!entity->getActiveState() || !camera->inCamera(entityRect))
		{
			continue;
		}
		entity->update(deltaTime, *collisionManager);
		if (entity->getType() == EntityType::PlayerType)
		{
			SDL_FRect playerRect = entity->getRect();
			camera->update(playerRect);
		}
	}
	for (const auto& entity : entities)
	{
		SDL_FRect entityRect = entity->getRect();
		if (!entity->getActiveState() || !camera->inCamera(entityRect))
		{
			continue;
		}
		entity->lateUpdate(deltaTime, *collisionManager);
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 92, 148, 252, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	tileMap->render(renderer, camera);
	for (const auto& entity : entities)
	{
		SDL_FRect entityRect = entity->getRect();
		if (!entity->getActiveState() || !camera->inCamera(entityRect))
		{
			continue;
		}
		entity->draw();
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