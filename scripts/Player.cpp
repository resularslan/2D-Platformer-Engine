#include "Player.h"
#include <iostream>
#include "Camera.h"

Player::Player()
	:
	playerSmallRunFrames{ nullptr, nullptr, nullptr },
	playerSmallIdleFrame(nullptr),
	playerSmallJumpFrame(nullptr),
	playerSmallSlideFrame(nullptr),
	playerSmallDeathFrame(nullptr),
	playerSmallFlag(nullptr),
	playerBigRunFrames{ nullptr, nullptr, nullptr },
	playerBigIdleFrame(nullptr),
	playerBigJumpFrame(nullptr),
	playerBigSlideFrame(nullptr),
	playerBigFlag(nullptr),
	playerMiddleFrame(nullptr)
{}

Player::~Player()
{
	SDL_DestroyTexture(playerSmallIdleFrame);
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(playerSmallRunFrames[i]);
	}
	SDL_DestroyTexture(playerSmallJumpFrame);
	SDL_DestroyTexture(playerSmallSlideFrame);
	SDL_DestroyTexture(playerSmallDeathFrame);
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(playerBigRunFrames[i]);
	}
	SDL_DestroyTexture(playerBigJumpFrame);
	SDL_DestroyTexture(playerBigSlideFrame);
}

void Player::init(SDL_Renderer* renderer)
{
	rect = { CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE , CELL_SIZE * 2, CELL_SIZE * 2 };
	playerSmallRunFrames[0] = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Run1.png");
	playerSmallRunFrames[1] = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Run2.png");
	playerSmallRunFrames[2] = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Run3.png");
	playerSmallJumpFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Jump.png");
	playerSmallIdleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Idle.png");
	playerSmallSlideFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Slide.png");
	playerSmallDeathFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Death.png");
	playerSmallFlag = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Flag.png");
	playerBigRunFrames[0] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run1.png");
	playerBigRunFrames[1] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run2.png");
	playerBigRunFrames[2] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run3.png");
	playerBigIdleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Idle.png");
	playerBigJumpFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Jump.png");
	playerBigSlideFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Slide.png");
	playerBigFlag = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Flag.png");
	playerMiddleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Middle.png");
	flip = SDL_FLIP_NONE;
	type = EntityType::PlayerType;
	runSpeed = 150;
	velocity.x = 0;
	velocity.y = 0;
}

void Player::movement(float deltaTime)
{
	velocity.x = runSpeed * deltaTime;
}

void Player::update(float deltaTime, CollisionManager& collisionManager)
{
	movement(deltaTime);
	Entity::update(deltaTime, collisionManager);
	if (isGrounded)
	{
		/*float frameDelay = velocity.x != 0 ? runAnimationSpeed / abs(velocity.x) : 0;*/
		float frameDelay = velocity.x != 0 ? 0.08f : 0;
		animation(3, frameDelay);
	}
	
}

void Player::onCollisionWithEntity(EntityType type, Direction direction)
{
	switch (type)
	{
		case EntityType::EnemyType:
			if (direction == Direction::Right || direction == Direction::Left || direction == Direction::Up)
			{
				isDied = true;
				// Die();
			}
			else
			{
				// jump
			}
			break;
		case EntityType::GrowMushroomType:
			isBig = true;
			break;
		case EntityType::HealthMushroomType:
			life++;
			break;
		case EntityType::StarType:
			starMode = true;
			// Star()
			break;
	}
}

void Player::draw(SDL_Renderer* renderer, Camera* camera)
{
	SDL_FRect newRect = {
		roundf(rect.x - camera->getRect().x),
		roundf(rect.y - camera->getRect().y),
		rect.w,
		rect.h
	};
	if (!camera->inCamera(rect))
	{
		return;
	}
	if (isBig)
	{
		if (isSliding)
		{
			SDL_RenderTextureRotated(renderer, playerBigSlideFrame, NULL, &newRect, 0, &center, flip);
		}
		else if (isJumping)
		{
			SDL_RenderTextureRotated(renderer, playerBigJumpFrame, NULL, &newRect, 0, &center, flip);
		}
		else if (isGrounded && velocity.x != 0)
		{
			SDL_RenderTextureRotated(renderer, playerBigRunFrames[frameIndex], NULL, &newRect, 0, &center, flip);
		}
		else
		{
			SDL_RenderTextureRotated(renderer, playerBigIdleFrame, NULL, &newRect, 0, &center, flip);
		}
	}
	else
	{
		if (isSliding)
		{
			SDL_RenderTextureRotated(renderer, playerSmallSlideFrame, NULL, &newRect, 0, &center, flip);
		}
		else if (isJumping)
		{
			SDL_RenderTextureRotated(renderer, playerSmallJumpFrame, NULL, &newRect, 0, &center, flip);
		}
		else if (isGrounded && velocity.x != 0)
		{
			SDL_RenderTextureRotated(renderer, playerSmallRunFrames[frameIndex], NULL, &newRect, 0, &center, flip);
		}
		else
		{
			SDL_RenderTextureRotated(renderer, playerSmallIdleFrame, NULL, &newRect, 0, &center, flip);
		}
	}
}

void Player::restart()
{
	rect = { CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE , CELL_SIZE * 2, CELL_SIZE * 2 };
	life--;
}