#include "Player.h"

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

void Player::movement(float deltaTime)
{
	oldVelocityX = velocity.x;
	return;
}

void Player::update(float deltaTime)
{
	return;
}

void Player::draw(SDL_Renderer* renderer, SDL_FRect* camera)
{
	SDL_FRect newRect = {
		roundf(rect.x - camera->x),
		roundf(rect.y - camera->y),
		rect.w,
		rect.h
	};
	if (isBig)
	{
		if (sliding)
		{
			SDL_RenderTextureRotated(renderer, playerBigSlideFrame, srcRect, &newRect, 0, center, flip);
		}
		else if (jumping)
		{
			SDL_RenderTextureRotated(renderer, playerBigJumpFrame, srcRect, &newRect, 0, center, flip);
		}
		else if (grounded)
		{
			float frameDelay = velocity.x != 0 ? runAnimationSpeed / abs(velocity.x) : 0
			animation(3, frameDelay);
			SDL_RenderTextureRotated(renderer, playerBigRunFrames[frameIndex], srcRect, &newRect, 0, center, flip);
		}
	}
	else
	{
		if (sliding)
		{
			SDL_RenderTextureRotated(renderer, playerSmallSlideFrame, srcRect, &newRect, 0, center, flip);
		}
		else if (jumping)
		{
			SDL_RenderTextureRotated(renderer, playerSmallJumpFrame, srcRect, &newRect, 0, center, flip);
		}
		else if (grounded)
		{
			float frameDelay = velocity.x != 0 ? runAnimationSpeed / abs(velocity.x) : 0
			animation(3, frameDelay);
			SDL_RenderTextureRotated(renderer, playerSmallRunFrames[frameIndex], srcRect, &newRect, 0, center, flip);
		}
	}
}

void Player::restart()
{
	rect = { CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE , CELL_SIZE * 2, CELL_SIZE * 2 };
	life--;
}

void Player::init()
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
}

void Player::~Player()
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