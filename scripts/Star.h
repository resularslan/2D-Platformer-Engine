#pragma once
#include "InteractableType.h"

class Star : public InteractableType
{
public:
	Star(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	~Star();
	void init() override;
	void draw() override;
protected:
	void verticalMovement(float deltaTime) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	SDL_Texture* texture;
	const float starGravity = 1000;
	const float frameDelay = 0.05f;
	const float starJumpForce = 370;
};
