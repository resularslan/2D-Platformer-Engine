#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Constants.h"
#include "MathUtils.h"

class Entity
{
protected:
	SDL_FRect rect;
	vector2 velocity;
	vector2 oldPos;
	void setOldPos();
	virtual void movement(float deltaTime) = 0;
public:
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void restart() = 0;
	virtual void draw(SDL_Renderer* renderer) = 0;
	virtual ~Entity() = default;
};