#include <Camera.h>

bool Camera::inCamera(SDL_FRect& other)
{
	if (other.x <= rect.x + rect.w && other.x + other.w >= rect.x &&
		other.y <= rect.y + rect.h && other.y + other.h >= rect.y)
	{
		return true;
	}
	return false;
}

void Camera::update(SDL_FRect& player)
{
	if (player.x > rect.x + (rect.w / 10 * 4))
	{
		rect.x = player.x - (rect.w / 10 * 4);
	}
	if (rect.x < 0) rect.x = 0;
	else if (rect.x + rect.w > WIDTH_CELL * CELL_SIZE) rect.x = WIDTH_CELL * CELL_SIZE - rect.w;
}