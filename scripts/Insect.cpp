#include "Insect.h"
#include "Player.h"

Insect::Insect(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Enemy(xPos, yPos, width, height, renderer, camera, id),
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
	Enemy::init();
	walkFrames[0] = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Walk1.png");
	walkFrames[1] = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Walk2.png");
	crushedFrame = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Crushed.png");
	deathFrame = IMG_LoadTexture(_renderer, "assets/Insect/Insect_Death.png");
	SDL_SetTextureScaleMode(walkFrames[0], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(walkFrames[1], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(crushedFrame, SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(deathFrame, SDL_SCALEMODE_NEAREST);
	currentState = InsectState::Alive;
	speed = walkSpeed;
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
		horizontalMovement(deltaTime);
		if (_camera.inCamera(rect))
		{
			velocity.y += gravity * deltaTime;
		}
		rect.x += velocity.x * deltaTime;
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
	Enemy::die();
	currentState = InsectState::Dying;
}

void Insect::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		if (dynamic_cast<Player*>(entity)->getStarModeState())
		{
			die();
			return;
		}
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

void Insect::crush()
{
	currentState = InsectState::Crushed;
	destroyTimer = 0;
}