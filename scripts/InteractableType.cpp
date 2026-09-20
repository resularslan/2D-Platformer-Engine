#include "InteractableType.h"

InteractableType::InteractableType(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, width, height, renderer, camera, id)
{}

void InteractableType::init()
{
	Entity::init();
	type = EntityType::InteractableType;
	currentState = InteractableTypeState::Spawning;
	isActive = false;
}

void InteractableType::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case InteractableTypeState::Spawning:
		spawnTimer += deltaTime;
		if (spawnTimer < spawnTime)
		{
			rect.y -= spawnSpeed * deltaTime;
		}
		else
		{
			currentState = InteractableTypeState::Alive;
		}
		break;
	case InteractableTypeState::Alive:
		Entity::update(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void InteractableType::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case InteractableTypeState::Spawning:
		break;
	case InteractableTypeState::Alive:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void InteractableType::restart()
{
	return;
}

ItemType InteractableType::getItemType()
{
	return itemType;
}

void InteractableType::spawn(float xPos, float yPos)
{
	isActive = true;
	rect.x = xPos;
	rect.y = yPos;
	updateCollisionRect();
}

void InteractableType::jump()
{
	velocity.y = -jumpForce;
	moveDirection.x = -moveDirection.x;
}

void InteractableType::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
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

void InteractableType::horizontalMovement(float deltaTime)
{
	velocity.x = moveDirection.x * speed;
}