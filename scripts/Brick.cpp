#include "Brick.h"
#include "Player.h"
#include "Enemy.h"
#include "InteractableType.h"
#include "CollisionManager.h"
#include "BrickPiece.h"

Brick::Brick(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, std::vector<BrickPiece*>& brickPieces)
	:
	BlockType(xPos, yPos, width, height, renderer, camera, id, collisionManager),
	brickTexture(nullptr),
	_brickPieces(brickPieces)
{}

Brick::~Brick()
{
	SDL_DestroyTexture(brickTexture);
}

void Brick::init()
{
	BlockType::init();
	brickTexture = IMG_LoadTexture(_renderer, "assets/TileMap/Brick.png");
	SDL_SetTextureScaleMode(brickTexture, SDL_SCALEMODE_NEAREST);
	currentState = BrickState::Static;
}

void Brick::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case BrickState::Static:
		break;
	case BrickState::Moving:
		moveTimer += deltaTime;
		if (moveTimer < moveTime / 2)
		{
			rect.y -= moveSpeed * deltaTime;
		}
		else if (moveTimer < moveTime)
		{
			rect.y += moveSpeed * deltaTime;
		}
		else
		{
			rect.y = originalY;
			currentState = BrickState::Static;
		}
		break;
	case BrickState::Breaking:
		destroyTimer += deltaTime;
		if (destroyTimer >= destroyTime)
		{
			isActive = false;
			int row = rect.y / (CELL_SIZE * 2);
			int col = rect.x / (CELL_SIZE * 2);
			_collisionManager.setCollisionType(row, col, CollisionType::None);
			createBrickPiece(-1, rect.x, rect.y, 450);
			createBrickPiece(-1, rect.x, rect.y + CELL_SIZE, 225);
			createBrickPiece(1, rect.x + CELL_SIZE, rect.y, 450);
			createBrickPiece(1, rect.x + CELL_SIZE, rect.y + CELL_SIZE, 225);
		}
		break;
	default:
		break;
	}
}

void Brick::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	Entity::lateUpdate(deltaTime, collisionManager);
}

void Brick::draw()
{
	switch (currentState)
	{
	case BrickState::Breaking:
		break;
	default:
		SDL_FRect newRect = _camera.adjustToCamera(rect);
		SDL_RenderTextureRotated(_renderer, brickTexture, NULL, &newRect, 0, &center, flip);
		break;
	}
}

void Brick::restart()
{
	return;
}

void Brick::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (currentState)
	{
	case BrickState::Static:
		switch (entity->getType())
		{
		case EntityType::PlayerType:
			switch (direction)
			{
			case Direction::Down:
				if (dynamic_cast<Player*>(entity)->getHitBlockRect().x != rect.x || dynamic_cast<Player*>(entity)->getHitBlockRect().y != rect.y)
				{
					return;
				}
				if (dynamic_cast<Player*>(entity)->getBigState())
				{
					breaking();
				}
				else
				{
					move();
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
	default:
		switch (entity->getType())
		{
		case EntityType::EnemyType:
			switch (direction)
			{
			case Direction::Up:
				dynamic_cast<Enemy*>(entity)->die();
				break;
			default:
				break;
			}
			break;
		case EntityType::InteractableType:
			switch (direction)
			{
			case Direction::Up:
				dynamic_cast<InteractableType*>(entity)->jump(rect.x);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	}
}

void Brick::createBrickPiece(float directionX, float posX, float posY, float jumpForce)
{
	for (auto& piece : _brickPieces)
	{
		if (!piece->getActiveState())
		{
			piece->setActiveState(true);
			piece->jump(jumpForce);
			piece->setDirectionX(directionX);
			piece->setPosition(posX, posY);
			break;
		}
	}
}

void Brick::move()
{
	BlockType::move();
	currentState = BrickState::Moving;
}

void Brick::breaking()
{
	currentState = BrickState::Breaking;
	destroyTimer = 0;
}
