#pragma once
#include "InteractableType.h"

class Flag : public InteractableType
{
public:
	Flag(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	~Flag();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	bool& getGroundedState();
	void flagAnimation();
protected:
	void horizontalMovement(float deltaTime) override;
private:
	SDL_Texture* texture;
	float flagVerticalSpeed = 0;
	const float flagDownSpeed = 150;
};
