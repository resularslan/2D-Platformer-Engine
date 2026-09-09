#include "GrowMushroom.h"
#include "Player.h"

GrowMushroom::GrowMushroom(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	texture(nullptr)
{}

GrowMushroom::~GrowMushroom()
{
	SDL_DestroyTexture(texture);
}

void GrowMushroom::init()
{
	Entity::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/GrowMushroom.png");
	type = EntityType::InteractableType;
	isActive = false;
}

void GrowMushroom::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case GrowMushroomState::Spawning:
		spawnTimer += deltaTime;
		if (spawnTimer < spawnTime)
		{
			rect.y -= spawnSpeed * deltaTime;
		}
		else
		{
			currentState = GrowMushroomState::Alive;
		}
		break;
	case GrowMushroomState::Alive:
		Entity::update(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void GrowMushroom::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case GrowMushroomState::Spawning:
		break;
	case GrowMushroomState::Alive:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void GrowMushroom::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, texture, NULL, &newRect, 0, &center, flip);
}

void GrowMushroom::restart()
{
	return;
}

void GrowMushroom::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
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

void GrowMushroom::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->grow();
		isActive = false;
		break;
	}
}

void GrowMushroom::horizontalMovement(float deltaTime)
{
	velocity.x = moveDirection.x * speed;
}
