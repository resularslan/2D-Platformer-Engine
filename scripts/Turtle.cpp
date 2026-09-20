#include "Turtle.h"
#include "Player.h"

Turtle::Turtle(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Enemy(xPos, yPos, width, height, renderer, camera, id),
	walkFrames{ nullptr, nullptr },
	sleepingFrame(nullptr),
	deathFrame(nullptr)
{}

Turtle::~Turtle()
{
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(sleepingFrame[i]);
	}
	SDL_DestroyTexture(deathFrame);
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(walkFrames[i]);
	}
}

void Turtle::init()
{
	Enemy::init();
	walkFrames[0] = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Walk1.png");
	walkFrames[1] = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Walk2.png");
	sleepingFrame[0] = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Sleeping.png");
	sleepingFrame[1] = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Waking.png");
	deathFrame = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Death.png");
	SDL_SetTextureScaleMode(walkFrames[0], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(walkFrames[1], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(sleepingFrame[0], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(sleepingFrame[1], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(deathFrame, SDL_SCALEMODE_NEAREST);
	currentState = TurtleState::Alive;
	speed = walkSpeed;
}

void Turtle::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case TurtleState::Alive:
		Entity::update(deltaTime, collisionManager);
		animation(2, walkAnimationFrameDelay, deltaTime);
		break;
	case TurtleState::Sleeping:
		Entity::update(deltaTime, collisionManager);
		wakeTimer += deltaTime;
		if (wakeTimer >= wakeAnimationTime)
		{
			animation(2, wakingAnimationFrameDelay, deltaTime);
		}
		if (wakeTimer >= wakeTime)
		{
			currentState = TurtleState::Alive;
			walkDirection = { -oldWalkDirectionX, walkDirection.y };
			speed = walkSpeed;
			rect.y -= CELL_SIZE;
			rect.h = CELL_SIZE * 3;
			updateCollisionRect();
		}
		break;
	case TurtleState::Sliding:
		Entity::update(deltaTime, collisionManager);
		if (wasInCamera && !_camera.inCamera(rect))
		{
			isActive = false;
		}
		wasInCamera = _camera.inCamera(rect);
		break;
	case TurtleState::Dying:
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

void Turtle::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case TurtleState::Dying:
		break;
	default:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	}
}

void Turtle::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	switch (currentState)
	{
	case TurtleState::Alive:
		SDL_RenderTextureRotated(_renderer, walkFrames[frameIndex], NULL, &newRect, 0, &center, flip);
		break;
	case TurtleState::Sleeping:
		SDL_RenderTextureRotated(_renderer, sleepingFrame[frameIndex], NULL, &newRect, 0, &center, flip);
		break;
	case TurtleState::Sliding:
		SDL_RenderTextureRotated(_renderer, sleepingFrame[0], NULL, &newRect, 0, &center, flip);
		break;
	case TurtleState::Dying:
		SDL_RenderTextureRotated(_renderer, deathFrame, NULL, &newRect, 0, &center, flip);
		break;
	default:
		break;
	}
}

void Turtle::restart()
{
	return;
}

void Turtle::die()
{
	Enemy::die();
	if (currentState == TurtleState::Alive)
	{
		rect.y += CELL_SIZE;
		rect.h = CELL_SIZE * 2;
	}
	currentState = TurtleState::Dying;
	updateCollisionRect();
}

void Turtle::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (currentState)
	{
	case TurtleState::Alive:
		switch (entity->getType())
		{
		case EntityType::PlayerType:
			switch (direction)
			{
			case Direction::Up:
				if (dynamic_cast<Player*>(entity)->getStarModeState())
				{
					die();
				}
				else
				{
					sleep();
					dynamic_cast<Player*>(entity)->jump(350);
				}
				break;
			default:
				if (dynamic_cast<Player*>(entity)->getStarModeState())
				{
					die();
				}
				else
				{
					dynamic_cast<Player*>(entity)->takeDamage();
				}
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
		break;
	case TurtleState::Sleeping:
		switch (entity->getType())
		{
		case EntityType::PlayerType:
			switch (direction)
			{
			case Direction::Up:
				if (entity->getCollisionRect().x + entity->getCollisionRect().w < collisionRect.x + (collisionRect.w / 2))
				{
					walkDirection = vector2::right;
				}
				else
				{
					walkDirection = vector2::left;
				}
				currentState = TurtleState::Sliding;
				speed = fastSpeed;
				break;
			case Direction::Right:
				walkDirection = vector2::left;
				currentState = TurtleState::Sliding;
				speed = fastSpeed;
				break;
			case Direction::Left:
				walkDirection = vector2::right;
				currentState = TurtleState::Sliding;
				speed = fastSpeed;
				break;
			default:
				break;
			}
			break;
		}
		break;
	case TurtleState::Sliding:
		switch (entity->getType())
		{
		case EntityType::PlayerType:
			switch (direction)
			{
			case Direction::Up:
				sleep();
				dynamic_cast<Player*>(entity)->jump(350);
				break;
			default:
				dynamic_cast<Player*>(entity)->takeDamage();
				break;
			}
			break;
		case EntityType::EnemyType:
			dynamic_cast<Enemy*>(entity)->die();
			break;
		default:
			break;
		}
		break;
	case TurtleState::Dying:
		break;
	default:
		break;
	}
}

void Turtle::sleep()
{
	if (currentState == TurtleState::Alive)
	{
		rect.y += CELL_SIZE;
		rect.h = CELL_SIZE * 2;
	}
	currentState = TurtleState::Sleeping;
	wakeTimer = 0;
	oldWalkDirectionX = walkDirection.x;
	walkDirection.x = 0;
	frameIndex = 0;
	updateCollisionRect();
}
