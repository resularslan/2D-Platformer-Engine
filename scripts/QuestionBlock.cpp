#include "QuestionBlock.h"
#include "Player.h"
#include "Enemy.h"
#include "CollisionManager.h"

QuestionBlock::QuestionBlock(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, int& commonFrameIndex, ItemType item, QuestionBlockState initialState, int itemCount, std::vector<InteractableType*>& interactableTypes)
	:
	BlockType(xPos, yPos, width, height, renderer, camera, id, collisionManager),
	lastTexture(nullptr),
	textures(nullptr, nullptr, nullptr, nullptr),
	emptyTexture(nullptr),
	brickTexture(nullptr),
	_commonFrameIndex(commonFrameIndex),
	_item(item),
	_itemCount(itemCount),
	_initialState(initialState),
	_interactableTypes(interactableTypes)
{}

QuestionBlock::~QuestionBlock()
{
	for (int i = 0; i < 3; i++)
	{
		SDL_DestroyTexture(textures[i]);
	}
	SDL_DestroyTexture(emptyTexture);
	SDL_DestroyTexture(brickTexture);
	SDL_DestroyTexture(lastTexture);
}

void QuestionBlock::init()
{
	BlockType::init();
	textures[0] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-1.png");
	textures[1] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-2.png");
	textures[2] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-3.png");
	textures[3] = IMG_LoadTexture(_renderer, "assets/TileMap/QuestionBlock-2.png");
	brickTexture = IMG_LoadTexture(_renderer, "assets/TileMap/Brick.png");
	emptyTexture = IMG_LoadTexture(_renderer, "assets/TileMap/EmptyBlock.png");
	for (int i = 0; i < 3; i++)
	{
		SDL_SetTextureScaleMode(textures[i], SDL_SCALEMODE_NEAREST);
	}
	SDL_SetTextureScaleMode(brickTexture, SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(emptyTexture, SDL_SCALEMODE_NEAREST);
	currentState = _initialState;
}

void QuestionBlock::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case QuestionBlockState::Moving:
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
			_itemCount--;
			rect.y = originalY;
			int row = rect.y / (CELL_SIZE * 2);
			int col = rect.x / (CELL_SIZE * 2);
			_collisionManager.setCollisionType(row, col, CollisionType::Solid);
			if (_itemCount == 0)
			{
				currentState = QuestionBlockState::Empty;
			}
			else
			{
				currentState = _initialState;
			}
		}
		break;
	default:
		break;
	}
}

void QuestionBlock::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case QuestionBlockState::Empty:
		break;
	default:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	}
	
}

void QuestionBlock::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	switch (currentState)
	{
	case QuestionBlockState::Invisible:
		lastTexture = nullptr;
		break;
	case QuestionBlockState::QuestionBlock:
		SDL_RenderTextureRotated(_renderer, textures[_commonFrameIndex], NULL, &newRect, 0, &center, flip);
		lastTexture = textures[_commonFrameIndex];
		break;
	case QuestionBlockState::Brick:
		SDL_RenderTextureRotated(_renderer, brickTexture, NULL, &newRect, 0, &center, flip);
		lastTexture = brickTexture;
		break;
	case QuestionBlockState::Moving:
		if (_itemCount > 1)
		{
			SDL_RenderTextureRotated(_renderer, lastTexture, NULL, &newRect, 0, &center, flip);
		}
		else if (_itemCount == 1)
		{
			SDL_RenderTextureRotated(_renderer, emptyTexture, NULL, &newRect, 0, &center, flip);
		}
		break;
	default:
		SDL_RenderTextureRotated(_renderer, emptyTexture, NULL, &newRect, 0, &center, flip);
		break;
	}
}

void QuestionBlock::restart()
{
	return;
}

void QuestionBlock::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (currentState)
	{
	case QuestionBlockState::Moving:
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
	case QuestionBlockState::Empty:
		break;
	default:
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
				spawnObject();
				move();
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

void QuestionBlock::move()
{
	BlockType::move();
	currentState = QuestionBlockState::Moving;
}

void QuestionBlock::spawnObject()
{
	for (auto& object : _interactableTypes)
	{
		if (object->getItemType() == _item)
		{
			if (!object->getActiveState())
			{
				object->spawn(rect.x, rect.y);
				break;
			}
		}
	}
}