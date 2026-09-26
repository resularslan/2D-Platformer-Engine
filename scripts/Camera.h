#pragma once

#include <SDL3/SDL.h>
#include <Constants.h>


class Camera
{
public:
	void init();
	bool inCamera(SDL_FRect& other);
	SDL_FRect adjustToCamera(SDL_FRect& other);
	void update(SDL_FRect player);
	SDL_FRect getRect();
private:
	SDL_FRect rect;
};