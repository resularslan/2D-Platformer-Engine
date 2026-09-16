#include "Star.h"
#include "Player.h"

Star::Star(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, width, height, renderer, camera, id),
	texture(nullptr)
{}

Star::~Star()
{
	SDL_DestroyTexture(texture);
}

void Star::init()
{
	Entity::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/Stars.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	type = EntityType::InteractableType;
	gravity = starGravity;
	isActive = false;
}

void Star::update(float deltaTime, CollisionManager& collisionManager)
{
	animation(4, frameDelay, deltaTime);
	switch (currentState)
	{
	case StarState::Spawning:
		spawnTimer += deltaTime;
		if (spawnTimer < spawnTime)
		{
			rect.y -= spawnSpeed * deltaTime;
		}
		else
		{
			currentState = StarState::Alive;
		}
		break;
	case StarState::Alive:
		Entity::update(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void Star::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case StarState::Spawning:
		break;
	case StarState::Alive:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	default:
		break;
	}
}

void Star::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_FRect srcRect = { frameIndex * 17.0f, 0, 16, 16 };
	SDL_RenderTextureRotated(_renderer, texture, &srcRect, &newRect, 0, &center, flip);
}

void Star::restart()
{
	return;
}

void Star::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
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

void Star::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->starMode();
		isActive = false;
		break;
	}
}

void Star::horizontalMovement(float deltaTime)
{
	velocity.x = moveDirection.x * speed;
}

void Star::verticalMovement(float deltaTime)
{
	if (isGrounded)
	{
		velocity.y = -jumpForce;
	}
}
