#pragma once

#include <SDL3/SDL.h>
#include <Constants.h>


class Camera
{
private:
	SDL_FRect rect;
public:
	bool inCamera(SDL_FRect& other);
	void update(SDL_FRect& player);
};