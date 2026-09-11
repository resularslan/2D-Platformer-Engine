#include "Brick.h"
#include "Player.h"

Brick::Brick(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	brickTexture(nullptr)
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
			collisionMargin = brickCollisionMargin;
		}
		updateCollisionRect();
		break;
	default:
		break;
	}
}

void Brick::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case BrickState::Static:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	case BrickState::Moving:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void Brick::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, brickTexture, NULL, &newRect, 0, &center, flip);
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
					// Break();
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
	case BrickState::Moving:
		switch (entity->getType())
		{
		case EntityType::EnemyType:
			entity->die();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	
}

void Brick::move()
{
	currentState = BrickState::Moving;
	moveTimer = 0;
	collisionMargin = { 8, 2 };
}
