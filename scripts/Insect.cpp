#include "Insect.h"
#include "Player.h"

Insect::Insect(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	walkFrames{ nullptr, nullptr },
	crushedFrame(nullptr),
	deathFrame(nullptr)
{}

Insect::~Insect()
{
	SDL_DestroyTexture(crushedFrame);
	SDL_DestroyTexture(deathFrame);
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(walkFrames[i]);
	}
}

void Insect::init()
{
	Entity::init();
	walkFrames[0] = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Walk1.png");
	walkFrames[1] = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Walk2.png");
	crushedFrame = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Crushed.png");
	deathFrame = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Death.png");
	type = EntityType::EnemyType;
	currentState = InsectState::Alive;
}

void Insect::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case InsectState::Alive:
		Entity::update(deltaTime, collisionManager);
		animation(2, walkAnimationFrameDelay, deltaTime);
		break;
	case InsectState::Crushed:
		destroyTimer += deltaTime;
		if (destroyTimer >= destroyTime)
		{
			isActive = false;
		}
		break;
	case InsectState::Dying:
		if (_camera.inCamera(rect))
		{
			velocity.y += gravity * deltaTime;
		}
		rect.y += velocity.y * deltaTime;
		break;
	default:
		break;
	}
}

void Insect::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case InsectState::Alive:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	case InsectState::Crushed:
		break;
	case InsectState::Dying:
		break;
	default:
		break;
	}
}

void Insect::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	switch (currentState)
	{
	case InsectState::Alive:
		SDL_RenderTextureRotated(_renderer, walkFrames[frameIndex], NULL, &newRect, 0, &center, flip);
		break;
	case InsectState::Crushed:
		SDL_RenderTextureRotated(_renderer, crushedFrame, NULL, &newRect, 0, &center, flip);
		break;
	case InsectState::Dying:
		SDL_RenderTextureRotated(_renderer, deathFrame, NULL, &newRect, 0, &center, flip);
		break;
	default:
		break;
	}
}

void Insect::restart()
{
	return;
}

void Insect::die()
{
	currentState = InsectState::Dying;
	if (_camera.inCamera(rect))
	{
		velocity.y = -deathJumpForce;
	}
	else
	{
		velocity.y = 0;
	}
	velocity.x = SDL_randf() > 0.5f ? walkDirection.x : -walkDirection.x;
}

void Insect::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
{
	Entity::onCollisionWithTile(direction, tileRect);
	switch (direction)
	{
	case Direction::Up:
		break;
	case Direction::Down:
		break;
	case Direction::Left:
		walkDirection = vector2::right;
		break;
	case Direction::Right:
		walkDirection = vector2::left;
		break;
	default:
		break;
	}
}

void Insect::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		switch (direction)
		{
		case Direction::Up:
			crush();
			dynamic_cast<Player*>(entity)->jump(350);
			break;
		default:
			dynamic_cast<Player*>(entity)->takeDamage();
			break;
		}
		break;
	case EntityType::EnemyType:
		switch (direction)
		{
		case Direction::Right:
			walkDirection = vector2::left;
			break;
		case Direction::Left:
			walkDirection = vector2::right;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Insect::horizontalMovement(float deltaTime)
{
	velocity.x = walkDirection.x * walkSpeed;
}

void Insect::crush()
{
	currentState = InsectState::Crushed;
	destroyTimer = 0;
}