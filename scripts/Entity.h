#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Constants.h"
#include "MathUtils.h"
#include "Camera.h"

class CollisionManager;

class Entity
{
public:
	Entity(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id);
	virtual ~Entity() = default;
	virtual void init() = 0;
	virtual void update(float deltaTime, CollisionManager& collisionManager) = 0;
	virtual void lateUpdate(float deltaTime, CollisionManager& collisionManager) = 0;
	virtual void restart() = 0;
	virtual void draw() = 0;
	virtual void die() {};
	SDL_FRect getRect();
	SDL_FRect getCollisionRect();
	vector2 getOldCollisionPos();
	EntityType getType();
	int getID();
	bool getActiveState();
	void spawn();
	bool isSpawnable();
	void setSpawnableState(bool state);
	bool getCollidableState();
protected:
	virtual void verticalMovement(float deltaTime) {};
	virtual void horizontalMovement(float deltaTime) {};
	virtual void onCollisionWithEntity(Entity* entity, Direction direction) {};
	virtual void onCollisionWithTile(Direction direction, SDL_FRect tileRect);
	void animation(int frameCount, float frameDelay, float deltaTime);
	void updateCollisionRect();
	void alignRect(SDL_FRect other, Direction direction);
	SDL_Renderer* _renderer;
	Camera& _camera;
	SDL_FRect rect;
	SDL_FRect collisionRect;
	vector2 collisionMargin = { 8, 2 };
	vector2 velocity = vector2::zero;
	vector2 oldCollisionPos;
	bool canCollide = true;
	bool isActive = true;
	bool isGrounded = false;
	bool canSpawn = false;
	float gravity = 1500;
	float maxVelocityY = 400;
	float _xPos;
	float _yPos;
	const float upCollisionErrorMargin = 16;
	EntityType type;
	int _id;
	SDL_FlipMode flip = SDL_FLIP_NONE;
	int frameIndex = 0;
	float animTimer = 0;
	float lastAnimTimer = 0;
	SDL_FPoint center;
};