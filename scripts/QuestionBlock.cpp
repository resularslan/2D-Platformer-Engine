#include "QuestionBlock.h"
#include "Player.h"
#include "CollisionManager.h"

QuestionBlock::QuestionBlock(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, int& commonFrameIndex)
	:
	Entity(xPos, yPos, renderer, camera, id),
	textures(nullptr, nullptr, nullptr),
	_collisionManager(collisionManager),
	_commonFrameIndex(commonFrameIndex)
{}

QuestionBlock::~QuestionBlock()
{
	for (int i = 0; i < 3; i++)
	{
		SDL_DestroyTexture(textures[i]);
	}
	SDL_DestroyTexture(emptyTexture);
}

void QuestionBlock::init()
{
	collisionMargin = questionBlockCollisionMargin;
	Entity::init();
	textures[0] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-1.png");
	textures[1] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-2.png");
	textures[2] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-3.png");
	textures[3] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-2.png");
	emptyTexture = IMG_LoadTexture(_renderer, "assets/TileMap/EmptyBlock.png");
	for (int i = 0; i < 3; i++)
	{
		SDL_SetTextureScaleMode(textures[i], SDL_SCALEMODE_NEAREST);
	}
	SDL_SetTextureScaleMode(emptyTexture, SDL_SCALEMODE_NEAREST);
	type = EntityType::BlockType;
	originalY = rect.y;
}

void QuestionBlock::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case QuestionBlockState::Loaded:
		break;
	case QuestionBlockState::Moving:
		moveTimer += deltaTime;
		if (moveTimer < moveTime / 2)
		{
			rect.y -= moveSpeed * deltaTime;
		}
		else if (moveTimer < moveTime)
		{
			rect.y += moveSpeed * deltaTime;
		}
		else
		{
			rect.y = originalY;
			currentState = QuestionBlockState::Empty;
			int row = rect.y / (CELL_SIZE * 2);
			int col = rect.x / (CELL_SIZE * 2);
			_collisionManager.setCollisionType(row, col, CollisionType::Solid);
		}
		break;
	case QuestionBlockState::Empty:
		break;
	default:
		break;
	}
}

void QuestionBlock::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case QuestionBlockState::Empty:
		break;
	default:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	}
	
}

void QuestionBlock::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	switch (currentState)
	{
	case QuestionBlockState::Empty:
		SDL_RenderTextureRotated(_renderer, emptyTexture, NULL, &newRect, 0, &center, flip);
		break;
	default:
		SDL_RenderTextureRotated(_renderer, textures[_commonFrameIndex], NULL, &newRect, 0, &center, flip);
		break;
	}
}

void QuestionBlock::restart()
{
	return;
}

void QuestionBlock::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (currentState)
	{
	case QuestionBlockState::Loaded:
		switch (entity->getType())
		{
		case EntityType::PlayerType:
			switch (direction)
			{
			case Direction::Down:
				if (dynamic_cast<Player*>(entity)->getHitBlockRect().x != rect.x || dynamic_cast<Player*>(entity)->getHitBlockRect().y != rect.y)
				{

					return;
				}
				move();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		switch (entity->getType())
		{
		case EntityType::EnemyType:
			switch (direction)
			{
			case Direction::Up:
				entity->die();
				break;
			default:
				break;
			}
			break;
		case EntityType::InteractableType:
			switch (direction)
			{
			case Direction::Up:
				entity->jump();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	}
}

void QuestionBlock::move()
{
	currentState = QuestionBlockState::Moving;
	moveTimer = 0;
}
