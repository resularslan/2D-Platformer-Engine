#include "HealthMushroom.h"
#include "Player.h"

HealthMushroom::HealthMushroom(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	texture(nullptr)
{}

HealthMushroom::~HealthMushroom()
{
	SDL_DestroyTexture(texture);
}

void HealthMushroom::init()
{
	Entity::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/healthMushroom.png");
	type = EntityType::InteractableType;
}

void HealthMushroom::update(float deltaTime, CollisionManager& collisionManager)
{
	Entity::update(deltaTime, collisionManager);
}

void HealthMushroom::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	Entity::lateUpdate(deltaTime, collisionManager);
}

void HealthMushroom::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, texture, NULL, &newRect, 0, &center, flip);
}

void HealthMushroom::restart()
{
	return;
}

void HealthMushroom::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
{
	Entity::onCollisionWithTile(direction, tileRect);
	switch (direction)
	{
	case Direction::Up:
		break;
	case Direction::Down:
		break;
	case Direction::Left:
		moveDirection = vector2::right;
		break;
	case Direction::Right:
		moveDirection = vector2::left;
		break;
	default:
		break;
	}
}

void HealthMushroom::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->addLife();
		isActive = false;
		break;
	}
}

void HealthMushroom::horizontalMovement(float deltaTime)
{
	velocity.x = moveDirection.x * speed;
}
