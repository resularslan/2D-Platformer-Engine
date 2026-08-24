#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Constants.h"
#include "MathUtils.h"

class Camera;
class CollisionManager;

class Entity
{
public:
	virtual ~Entity() = default;
	virtual void init(SDL_Renderer* renderer) = 0;
	virtual void update(float deltaTime, CollisionManager& collisionManager) = 0;
	virtual void restart() = 0;
	virtual void draw(SDL_Renderer* renderer, Camera* camera) = 0;
	SDL_FRect getRect();
	SDL_FRect getCollisionRect();
	EntityType getType();
	int getID();
	void setID(int newID);
protected:
	virtual void verticalMovement(float deltaTime) = 0;
	virtual void horizontalMovement(float deltaTime) = 0;
	virtual void onCollisionWithEntity(Entity* entity, Direction direction) {};
	virtual void onCollisionWithTile(Direction direction) = 0;
	void animation(int frameCount, float frameDelay, float deltaTime);
	void updateCollisionRect();
	SDL_FRect rect;
	SDL_FRect collisionRect;
	vector2 collisionMargin = { 16, 4 };
	vector2 velocity = vector2::zero;
	vector2 oldCollisionPos;
	bool isActive = true;
	bool isGrounded = false;
	float gravity = 600;
	EntityType type;
	int id;
	SDL_FlipMode flip;
	int frameIndex = 0;
	float animTimer = 0;
	float lastAnimTimer = 0;
	SDL_FPoint center;
};