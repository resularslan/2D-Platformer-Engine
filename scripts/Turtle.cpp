#include "Turtle.h"
#include "Player.h"

Turtle::Turtle(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	walkFrames{ nullptr, nullptr },
	sleepingFrame(nullptr),
	deathFrame(nullptr)
{}

Turtle::~Turtle()
{
	SDL_DestroyTexture(sleepingFrame);
	SDL_DestroyTexture(deathFrame);
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(walkFrames[i]);
	}
}

void Turtle::init()
{
	Entity::init();
	walkFrames[0] = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Walk1.png");
	walkFrames[1] = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Walk2.png");
	sleepingFrame = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Sleeping.png");
	deathFrame = IMG_LoadTexture(_renderer, "assets/Turtle/Turtle_Death.png");
	type = EntityType::EnemyType;
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
		if (walkDirection.x == 0)
		{
			wakeTimer += deltaTime;
		}
		if (wakeTimer >= wakeTime)
		{
			currentState = TurtleState::Alive;
			walkDirection = { oldWalkDirectionX, walkDirection.y };
			speed = walkSpeed;
		}
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
	case TurtleState::Alive:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	case TurtleState::Sleeping:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	case TurtleState::Dying:
		break;
	default:
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
		SDL_RenderTextureRotated(_renderer, sleepingFrame, NULL, &newRect, 0, &center, flip);
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
	currentState = TurtleState::Dying;
	speed = deathSpeed;
	canCollide = false;
	if (_camera.inCamera(rect))
	{
		velocity.y = -deathJumpForce;
	}
	else
	{
		velocity.y = 0;
	}
	SDL_srand(0);
	velocity.x = SDL_rand(1) == 1 ? walkDirection.x : -walkDirection.x;
}

void Turtle::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
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
				if (walkDirection.x == 0)
				{
					speed = fastSpeed;
					if (entity->getCollisionRect().x + entity->getCollisionRect().w < collisionRect.x + (collisionRect.w / 2))
					{
						walkDirection = vector2::right;
					}
					else
					{
						walkDirection = vector2::left;
					}
				}
				else
				{
					walkDirection = vector2::zero;
				}
				break;
			case Direction::Right:
				if (walkDirection.x == 0)
				{
					walkDirection = vector2::left;
					speed = fastSpeed;
				}
				else
				{
					dynamic_cast<Player*>(entity)->takeDamage();
				}
				break;
			case Direction::Left:
				if (walkDirection.x == 0)
				{
					walkDirection = vector2::right;
					speed = fastSpeed;
				}
				else
				{
					dynamic_cast<Player*>(entity)->takeDamage();
				}
				break;
			default:
				break;
			}
			break;
		case EntityType::EnemyType:
			switch (direction)
			{
			case Direction::Right:
				if (velocity.x != 0)
				{
					entity->die();
				}
				break;
			case Direction::Left:
				if (velocity.x != 0)
				{
					entity->die();
				}
				break;
			default:
				break;
			}
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

void Turtle::horizontalMovement(float deltaTime)
{
	velocity.x = walkDirection.x * speed;
}

void Turtle::sleep()
{
	currentState = TurtleState::Sleeping;
	wakeTimer = 0;
	oldWalkDirectionX = walkDirection.x;
	walkDirection.x = 0;
}
