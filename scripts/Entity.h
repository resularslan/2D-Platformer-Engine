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
	SDL_FlipMode flip;
	int frameIndex = 0;
	Uint32 animTimer = 0;
	Uint32 lastAnimTimer = 0;
	SDL_FPoint center = { CELL_SIZE, CELL_SIZE };
	void setOldPos();
	void animation(int frameCount, float frameDelay);
	virtual void movement(float deltaTime) = 0;
public:
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void restart() = 0;
	virtual void draw(SDL_Renderer* renderer, SDL_FRect* camera) = 0;
	virtual ~Entity() = default;
};