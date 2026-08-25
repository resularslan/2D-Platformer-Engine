#include "Entity.h"
#include "CollisionManager.h"

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
	rect.x += velocity.x * deltaTime;
	updateCollisionRect();
	std::vector<TileCollisionInfo> xTileCollisions = collisionManager.tileCollision(*this);
	std::vector<Entity*> xEntityCollisions = collisionManager.entityCollision(*this);
	if (xTileCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (auto& tile : xTileCollisions)
			{
				if (tile.collisionType == Solid)
				{
					if (oldCollisionPos.x + collisionRect.w <= tile.tilePos.x)
					{
						alignRect(tile.tilePos, Direction::Left);
						onCollisionWithTile(Direction::Right);
						break;
					}
				}
			}
		}
		else if (velocity.x < 0)
		{
			for (auto& tile : xTileCollisions)
			{
				if (tile.collisionType == Solid)
				{
					if (oldCollisionPos.x >= tile.tilePos.x + tile.tilePos.w)
					{
						alignRect(tile.tilePos, Direction::Right);
						onCollisionWithTile(Direction::Left);
						break;
					}
				}
			}
		}
	}
	if (xEntityCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				if (oldCollisionPos.x + collisionRect.w <= entity->getCollisionRect().x)
				{
					onCollisionWithEntity(entity, Right);
				}
			}
		}
		else if (velocity.x < 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				if (oldCollisionPos.x >= entity->getCollisionRect().x + entity->getCollisionRect().w)
				{
					onCollisionWithEntity(entity, Direction::Left);
				}
			}
		}
	}
	rect.y += velocity.y * deltaTime;
	updateCollisionRect();
	std::vector<TileCollisionInfo> yTileCollisions = collisionManager.tileCollision(*this);
	std::vector<Entity*> yEntityCollisions = collisionManager.entityCollision(*this);
	if (yTileCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			for (auto& tile : yTileCollisions)
			{
				if (tile.collisionType == Solid)
				{
					if (oldCollisionPos.y + collisionRect.h <= tile.tilePos.y)
					{
						alignRect(tile.tilePos, Direction::Up);
						onCollisionWithTile(Direction::Down);
						break;
					}
				}
			}
		}
		else if (velocity.y < 0)
		{
			for (auto& tile : yTileCollisions)
			{
				if (tile.collisionType == Solid || tile.collisionType == SolidFromBottom)
				{
					if (oldCollisionPos.y >= tile.tilePos.y + tile.tilePos.h)
					{
						if (collisionRect.x + (collisionRect.w / 2) < tile.tilePos.x)
						{
							rect.x -= 250 * deltaTime;
							updateCollisionRect();
						}
						else if (collisionRect.x + (collisionRect.w / 2) > tile.tilePos.x + tile.tilePos.w)
						{
							rect.x += 250 * deltaTime;
							updateCollisionRect();
						}
						else
						{
							velocity.y = 0;
							alignRect(tile.tilePos, Direction::Down);
						}
						onCollisionWithTile(Direction::Up);
						break;
					}
				}
			}
		}
	}
	if (yEntityCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			for (auto& entity : yEntityCollisions)
			{
				if (oldCollisionPos.y + collisionRect.h <= entity->getCollisionRect().y)
				{
					onCollisionWithEntity(entity, Direction::Down);
				}
			}
		}
		else if (velocity.y < 0)
		{
			for (auto& entity : yEntityCollisions)
			{
				if (oldCollisionPos.y >= entity->getCollisionRect().y + entity->getCollisionRect().h)
				{
					onCollisionWithEntity(entity, Direction::Up);
				}
			}
		}
	}
	updateCollisionRect();
	oldCollisionPos = { collisionRect.x , collisionRect.y };
}

SDL_FRect Entity::getRect()
{
	return rect;
}

SDL_FRect Entity::getCollisionRect()
{
	return collisionRect;
}

EntityType Entity::getType()
{
	return type;
}

int Entity::getID()
{
	return id;
}

void Entity::setID(int newID)
{
	id = newID;
}

void Entity::onCollisionWithTile(Direction direction)
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