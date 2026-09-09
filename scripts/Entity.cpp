#include "Entity.h"
#include "CollisionManager.h"

Entity::Entity(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	_renderer(renderer),
	_camera(camera),
	_xPos(xPos),
	_yPos(yPos),
	_id(id)
{ }

void Entity::init()
{
	rect = { _xPos, _yPos , CELL_SIZE * 2, CELL_SIZE * 2 };
	updateCollisionRect();
	center = { rect.w / 2, rect.h / 2 };
	oldCollisionPos = { collisionRect.x, collisionRect.y };
	flip = SDL_FLIP_NONE;
}

void Entity::update(float deltaTime, CollisionManager& collisionManager)
{
	horizontalMovement(deltaTime);
	verticalMovement(deltaTime);
	isGrounded = false;
	if (velocity.y < maxVelocityY)
	{
		velocity.y += gravity * deltaTime;
		if (velocity.y > maxVelocityY)
		{
			velocity.y = maxVelocityY;
		}
	}
	oldCollisionPos = { collisionRect.x , collisionRect.y };
	rect.x += velocity.x * deltaTime;
	updateCollisionRect();
	std::vector<TileCollisionInfo> xTileCollisions = collisionManager.tileCollision(*this);
	if (xTileCollisions.size() > 0)
	{
		for (auto& tile : xTileCollisions)
		{
			if (velocity.x > 0)
			{
				if (tile.collisionType == CollisionType::Solid)
				{
					if (oldCollisionPos.x + collisionRect.w <= tile.tileRect.x)
					{
						alignRect(tile.tileRect, Direction::Left);
						onCollisionWithTile(Direction::Right, tile.tileRect);
						break;
					}
				}
			}
			else if (velocity.x < 0)
			{
				if (tile.collisionType == CollisionType::Solid)
				{
					if (oldCollisionPos.x >= tile.tileRect.x + tile.tileRect.w)
					{
						alignRect(tile.tileRect, Direction::Right);
						onCollisionWithTile(Direction::Left, tile.tileRect);
						break;
					}
				}
			}
		}
	}
	rect.y += velocity.y * deltaTime;
	updateCollisionRect();
	std::vector<TileCollisionInfo> yTileCollisions = collisionManager.tileCollision(*this);
	if (yTileCollisions.size() > 0)
	{
		for (auto& tile : yTileCollisions)
		{
			if (velocity.y > 0)
			{
				if (tile.collisionType == CollisionType::Solid)
				{
					if (oldCollisionPos.y + collisionRect.h <= tile.tileRect.y)
					{
						alignRect(tile.tileRect, Direction::Up);
						onCollisionWithTile(Direction::Down, tile.tileRect);
						break;
					}
				}
			}
			else if (velocity.y < 0)
			{
				if (tile.collisionType == CollisionType::Solid || tile.collisionType == CollisionType::SolidFromBottom)
				{
					if (oldCollisionPos.y >= tile.tileRect.y + tile.tileRect.h)
					{
						if (collisionRect.x + upCollisionErrorMargin < tile.tileRect.x && yTileCollisions.size() == 1)
						{
							alignRect(tile.tileRect, Direction::Left);
						}
						else if (collisionRect.x + collisionRect.w > tile.tileRect.x + tile.tileRect.w + upCollisionErrorMargin && yTileCollisions.size() == 1)
						{
							alignRect(tile.tileRect, Direction::Right);
						}
						else
						{
							velocity.y = 0;
							alignRect(tile.tileRect, Direction::Down);
							onCollisionWithTile(Direction::Up, tile.tileRect);
						}
						break;
					}
				}
			}
		}
	}
	updateCollisionRect();
}

void Entity::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	std::vector<Entity*> entityCollisions = collisionManager.entityCollision(*this);
	if (entityCollisions.size() > 0)
	{
		for (auto& entity : entityCollisions)
		{
			if (oldCollisionPos.y + collisionRect.h <= entity->getOldCollisionPos().y)
			{
				onCollisionWithEntity(entity, Direction::Down);
			}
			if (oldCollisionPos.y >= entity->getOldCollisionPos().y + entity->getCollisionRect().h)
			{
				onCollisionWithEntity(entity, Direction::Up);
			}
			if (oldCollisionPos.x >= entity->getOldCollisionPos().x + entity->getCollisionRect().w)
			{
				onCollisionWithEntity(entity, Direction::Left);
			}
			if (oldCollisionPos.x + collisionRect.w <= entity->getOldCollisionPos().x)
			{
				onCollisionWithEntity(entity, Direction::Right);
			}
		}
	}
}

SDL_FRect Entity::getRect()
{
	return rect;
}

SDL_FRect Entity::getCollisionRect()
{
	return collisionRect;
}

vector2 Entity::getOldCollisionPos()
{
	return oldCollisionPos;
}

EntityType Entity::getType()
{
	return type;
}

int Entity::getID()
{
	return _id;
}

bool Entity::getActiveState()
{
	return isActive;
}

void Entity::spawn()
{
	isActive = true;
	canSpawn = false;
}

bool Entity::isSpawnable()
{
	return canSpawn;
}

void Entity::setSpawnableState(bool state)
{
	canSpawn = state;
}

void Entity::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
{
	switch (direction)
	{
	case Direction::Up:
		break;
	case Direction::Down:
		isGrounded = true;
		velocity.y = 0;
		break;
	case Direction::Left:
		break;
	case Direction::Right:
		break;
	default:
		break;
	}
}

void Entity::animation(int frameCount, float frameDelay, float deltaTime)
{
	animTimer += deltaTime;
	if (animTimer > lastAnimTimer + frameDelay)
	{
		lastAnimTimer =  animTimer;
		frameIndex = (frameIndex + 1) % frameCount;
	}
}

void Entity::updateCollisionRect()
{
	collisionRect = { rect.x + (collisionMargin.x / 2), rect.y + (collisionMargin.y / 2), rect.w - collisionMargin.x, rect.h - collisionMargin.y };
}

void Entity::alignRect(SDL_FRect other, Direction direction)
{
	switch (direction)
	{
	case Direction::Down:
		rect.y = other.y + other.h - (collisionMargin.y / 2);
		break;
	case Direction::Up:
		rect.y = other.y - rect.h + (collisionMargin.y / 2);
		break;
	case Direction::Right:
		rect.x = other.x + other.w - (collisionMargin.x / 2);
		break;
	case Direction::Left:
		rect.x = other.x - rect.w + (collisionMargin.x / 2);
		break;
	default:
		break;
	}
}