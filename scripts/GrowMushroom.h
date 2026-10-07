#pragma once
#include "InteractableType.h"

class GrowMushroom : public InteractableType
{
public:
	GrowMushroom(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	~GrowMushroom();
	void init() override;
	void draw() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	SDL_Texture* texture;
};
