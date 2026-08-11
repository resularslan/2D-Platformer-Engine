#include <Camera.h>

void Camera::init()
{
	rect.x = 0;
	rect.y = 0;
	rect.w = WINDOW_WIDTH;
	rect.h = WINDOW_HEIGHT;
}

bool Camera::inCamera(SDL_FRect& other)
{
	if (other.x <= rect.x + rect.w && other.x + other.w >= rect.x &&
		other.y <= rect.y + rect.h && other.y + other.h >= rect.y)
	{
		return true;
	}
	return false;
}

void Camera::update(/*SDL_FRect& player*/ float deltaTime)
{
	///*if (player.x > rect.x + (rect.w / 10 * 4))
	//{
	//	rect.x = player.x - (rect.w / 10 * 4);
	//}
	if (rect.x >= 0 && rect.x + rect.w < MAP_WIDTH_TILE * CELL_SIZE)
	{
		rect.x += 300 * deltaTime;
	}
	if (rect.x < 0) rect.x = 0;
	else if (rect.x + rect.w > MAP_WIDTH_TILE * CELL_SIZE) rect.x = MAP_WIDTH_TILE * CELL_SIZE - rect.w;
}

SDL_FRect Camera::getRect()
{
	return rect;
}