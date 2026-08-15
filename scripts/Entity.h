#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Constants.h"
#include "MathUtils.h"
#include <vector>

struct EntityType
{
	Player,
	Insect,
	Turtle,
	GrowMushroom,
	HealthMushroom,
	Star
};

class Entity
{
public:
	virtual ~Entity() = default;
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void restart() = 0;
	virtual void draw(SDL_Renderer* renderer, SDL_FRect* camera) = 0;
	/*vector2 getOldPos();*/
	vector2 getRect();
	EntityType getType();
	int getID();
	void setID(int newID);
protected:
	virtual void movement(float deltaTime) = 0;
	virtual void onCollisionWithEntity(EntityType type, Direction directon)
	/*void setOldPos();*/
	void animation(int frameCount, float frameDelay);
	SDL_FRect rect;
	vector2 velocity;
	/*vector2 oldPos;*/
	bool isGrounded;
	EntityType type;
	int id;
	SDL_FlipMode flip;
	int frameIndex = 0;
	Uint32 animTimer = 0;
	Uint32 lastAnimTimer = 0;
	SDL_FPoint center = { rect.w / 2, rect.h / 2 };
};