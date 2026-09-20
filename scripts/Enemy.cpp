#include "Enemy.h"

Enemy::Enemy(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, width, height, renderer, camera, id)
{}

void Enemy::init()
{
	Entity::init();
	type = EntityType::EnemyType;
}

void Enemy::restart()
{
	return;
}

void Enemy::die()
{
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
	int randomNumber = SDL_rand(2);
	walkDirection.x = randomNumber == 1 ? 1 : -1;
}

void Enemy::onCollisionWithTile(Direction direction, SDL_FRect tileRect)
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

void Enemy::horizontalMovement(float deltaTime)
{
	flip = velocity.x > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	velocity.x = walkDirection.x * speed;
}