#pragma once
#include "Entity.h"

class BrickPiece : public Entity
{
public:
	BrickPiece(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id);
	~BrickPiece();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
	void jump(float force);
	void setDirectionX(float x);
	void setPosition(float x, float y);
	void setActiveState(float state);
protected:
	void horizontalMovement(float deltaTime) override;
private:
	SDL_Texture* textures[2];
	vector2 brickCollisionMargin = { 0, 0 };
	vector2 direction = { 1, 0 };
	const float speed = 100;
};
