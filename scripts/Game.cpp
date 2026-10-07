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
	SDL_srand(0);
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
	turtleInfo = { 214 * CELL_SIZE, WINDOW_HEIGHT - 7 * CELL_SIZE };
	brickInfos[0] = { 40 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[1] = { 44 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[2] = { 48 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[3] = { 154 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[4] = { 158 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[5] = { 160 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[6] = { 162 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[7] = { 164 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[8] = { 166 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[9] = { 168 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[10] = { 170 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[11] = { 172 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[12] = { 174 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[13] = { 182 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[14] = { 184 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[15] = { 186 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[16] = { 200 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[17] = { 236 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[18] = { 242 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[19] = { 244 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[20] = { 246 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[21] = { 256 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[22] = { 258 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[23] = { 260 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[24] = { 262 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE };
	brickInfos[25] = { 336 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[26] = { 338 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	brickInfos[27] = { 342 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE };
	questionBlockInfos[0] = { 32 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[1] = { 42 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::GrowMushroom, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[2] = { 44 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[3] = { 46 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[4] = { 128 * CELL_SIZE, WINDOW_HEIGHT - 14 * CELL_SIZE, ItemType::HealthMushroom, QuestionBlockState::Invisible, 1 };
	questionBlockInfos[5] = { 156 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::GrowMushroom, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[6] = { 188 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[7] = { 188 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::Brick, 5 };
	questionBlockInfos[8] = { 202 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Star, QuestionBlockState::Brick, 1 };
	questionBlockInfos[9] = { 212 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[10] = { 218 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[11] = { 218 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE, ItemType::GrowMushroom, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[12] = { 224 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[13] = { 258 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[14] = { 260 * CELL_SIZE, WINDOW_HEIGHT - 20 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	questionBlockInfos[15] = { 340 * CELL_SIZE, WINDOW_HEIGHT - 12 * CELL_SIZE, ItemType::Coin, QuestionBlockState::QuestionBlock, 1 };
	camera = new Camera();
	tileMap = new TileMap();
	collisionManager = new CollisionManager(entities);
	flag = std::make_unique<Flag>((MAP_WIDTH_TILE - 29) * CELL_SIZE, WINDOW_HEIGHT - 24 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++);
	player = std::make_unique<Player>(keys, 6 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++, flag.get()->getGroundedState(), restartTheGame, stopTheGame);
	for (int i = 0; i < insectCount; i++)
	{
		insects[i] = std::make_unique<Insect>(insectInfos[i].xPos, insectInfos[i].yPos, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++);
		entities.push_back(std::move(insects[i]));
	}
	turtle = std::make_unique<Turtle>(turtleInfo.xPos, turtleInfo.yPos, CELL_SIZE * 2, CELL_SIZE * 3, renderer, *camera, entityCount++);
	entities.push_back(std::move(turtle));
	for (int i = 0; i < growMushroomCount; i++)
	{
		growMushrooms[i] = std::make_unique<GrowMushroom>(-99 * CELL_SIZE, -99 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++);
		interactableTypes.push_back(growMushrooms[i].get());
		entities.push_back(std::move(growMushrooms[i]));
	}
	for (int i = 0; i < healthMushroomCount; i++)
	{
		healthMushrooms[i] = std::make_unique<HealthMushroom>(-99 * CELL_SIZE, -99 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++);
		interactableTypes.push_back(healthMushrooms[i].get());
		entities.push_back(std::move(healthMushrooms[i]));
	}
	for (int i = 0; i < starCount; i++)
	{
		stars[i] = std::make_unique<Star>(-99 * CELL_SIZE, -99 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++);
		interactableTypes.push_back(stars[i].get());
		entities.push_back(std::move(stars[i]));
	}
	for (int i = 0; i < coinCount; i++)
	{
		coins[i] = std::make_unique<Coin>(-99 * CELL_SIZE, -99 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++, player.get());
		interactableTypes.push_back(coins[i].get());
		entities.push_back(std::move(coins[i]));
	}
	for (int i = 0; i < brickPieceCount; i++)
	{
		brickPieces[i] = std::make_unique<BrickPiece>(-99 * CELL_SIZE, -99 * CELL_SIZE, CELL_SIZE, CELL_SIZE, renderer, *camera, entityCount++);
		brickPiecesOriginals.push_back(brickPieces[i].get());
		entities.push_back(std::move(brickPieces[i]));
	}
	for (int i = 0; i < brickCount; i++)
	{
		bricks[i] = std::make_unique<Brick>(brickInfos[i].xPos, brickInfos[i].yPos, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++, *collisionManager, brickPiecesOriginals);
		entities.push_back(std::move(bricks[i]));
	}
	for (int i = 0; i < questionBlockCount; i++)
	{
		questionBlocks[i] = std::make_unique<QuestionBlock>(questionBlockInfos[i].xPos, questionBlockInfos[i].yPos, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++, *collisionManager, commonFrameIndex, questionBlockInfos[i].item, questionBlockInfos[i].initialState, questionBlockInfos[i].itemCount, interactableTypes);
		entities.push_back(std::move(questionBlocks[i]));
	}
	flagPole = std::make_unique<FlagPole>((MAP_WIDTH_TILE - 28) * CELL_SIZE, WINDOW_HEIGHT - 26 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 20, renderer, *camera, entityCount++, *collisionManager, *flag.get());
	entities.push_back(std::move(flagPole));
	entities.push_back(std::move(flag));
	finishDoor = std::make_unique<FinishDoor>((MAP_WIDTH_TILE - 14) * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2, renderer, *camera, entityCount++, *collisionManager);
	entities.push_back(std::move(finishDoor));
	entities.push_back(std::move(player));
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
	restart();
	commonAnimTimer += deltaTime;
	if (commonFrameIndex == 0)
	{
		if (commonAnimTimer > commonLastAnimTimer + commonFirstFrameDelay)
		{
			commonLastAnimTimer = commonAnimTimer;
			commonFrameIndex = 1;
		}
	}
	else
	{
		if (commonAnimTimer > commonLastAnimTimer + commonFrameDelay)
		{
			commonLastAnimTimer = commonAnimTimer;
			commonFrameIndex = (commonFrameIndex + 1) % 4;
		}
	}
	for (const auto& entity : entities)
	{
		SDL_FRect entityRect = entity->getRect();
		if (!entity->getActiveState() || (!camera->inCamera(entityRect) && entity->getType() != EntityType::PlayerType))
		{
			continue;
		}
		if (entity->getType() == EntityType::PlayerType)
		{
			camera->update(entity->getRect());
			if (stopTheGame)
			{
				entity->update(deltaTime, *collisionManager);
			}
		}
		if (!stopTheGame)
		{
			entity->update(deltaTime, *collisionManager);
		}
	}
	for (const auto& entity : entities)
	{
		SDL_FRect entityRect = entity->getRect();
		if (!entity->getActiveState() || (!camera->inCamera(entityRect) && entity->getType() != EntityType::PlayerType))
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
		if (!entity->getActiveState() || (!camera->inCamera(entityRect) && entity->getType() != EntityType::PlayerType))
		{
			continue;
		}
		entity->draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::restart()
{
	if (restartTheGame)
	{
		tileMap->restart();
		collisionManager->restart();
		camera->restart();
		for (const auto& entity : entities)
		{
			entity->restart();
		}
		restartTheGame = false;
	}
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