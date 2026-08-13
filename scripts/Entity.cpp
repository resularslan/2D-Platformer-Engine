#include "Entity.h"

void Entity::getOldPos()
{
	return rect;
}

void Entity::update(float deltaTime, CollsionManager& collisionManager)
{
	setOldPos();
	velocity.y += gravity * deltaTime;
	rect.x += velocity.x;
	if (collisionManager->tileCollision(this))
	{
		if (velocity.x > 0)
		{

		}
		else
		{

		}
	}
	rect.y += velocity.y;
	if (collisionManager->tileCollision(this))
	{
		if (velocity.y > 0)
		{

		}
		else
		{

		}
	}
}

void Entity::setOldPos()
{
	oldPos.x = rect.x;
	oldPos.y = rect.y;
}

void Entity::animation(int frameCount, float frameDelay)
{
	if (animTimer > lastAnimTimer + frameDelay)
	{
		lastAnimTimer = currentTime;
		frameIndex = (frameIndex + 1) % frameCount;
	}
}