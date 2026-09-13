#include "Brick.h"
#include "Player.h"
#include "CollisionManager.h"

Brick::Brick(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager)
	:
	Entity(xPos, yPos, renderer, camera, id),
	brickTexture(nullptr),
	_collisionManager(collisionManager)
{}

Brick::~Brick()
{
	SDL_DestroyTexture(brickTexture);
}

void Brick::init()
{
	collisionMargin = brickCollisionMargin;
	Entity::init();
	brickTexture = IMG_LoadTexture(_renderer, "assets/TileMap/Brick.png");
	type = EntityType::InteractableType;
	originalY = rect.y;
}

void Brick::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case BrickState::Static:
		break;
	case BrickState::Moving:
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
			currentState = BrickState::Static;
		}
		break;
	case BrickState::Breaking:
		destroyTimer += deltaTime;
		if (destroyTimer >= destroyTime)
		{
			isActive = false;
			int row = rect.y / (CELL_SIZE * 2);
			int col = rect.x / (CELL_SIZE * 2);
			_collisionManager.setCollisionType(row, col, CollisionType::None);
		}
		break;
	default:
		break;
	}
}

void Brick::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	Entity::lateUpdate(deltaTime, collisionManager);
}

void Brick::draw()
{
	switch (currentState)
	{
	case BrickState::Breaking:
		break;
	default:
		SDL_FRect newRect = _camera.adjustToCamera(rect);
		SDL_RenderTextureRotated(_renderer, brickTexture, NULL, &newRect, 0, &center, flip);
		break;
	}
}

void Brick::restart()
{
	return;
}

void Brick::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (currentState)
	{
	case BrickState::Static:
		switch (entity->getType())
		{
		case EntityType::PlayerType:
			switch (direction)
			{
			case Direction::Down:
				if (dynamic_cast<Player*>(entity)->getBigState())
				{
					breaking();
				}
				else
				{
					move();
				}
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

void Brick::move()
{
	currentState = BrickState::Moving;
	moveTimer = 0;
}

void Brick::breaking()
{
	currentState = BrickState::Breaking;
	destroyTimer = 0;
}
