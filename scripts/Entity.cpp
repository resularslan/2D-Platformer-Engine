#include "Entity.h"

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