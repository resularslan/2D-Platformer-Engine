#include "Player.h"
#include "Camera.h"

Player::Player(const std::array<bool, SDL_SCANCODE_COUNT>& keys, Camera& camera)
	:
	playerSmallRunFrames{ nullptr, nullptr, nullptr },
	playerSmallIdleFrame(nullptr),
	playerSmallJumpFrame(nullptr),
	playerSmallSlideFrame(nullptr),
	playerDeathFrame(nullptr),
	playerSmallFlag(nullptr),
	playerBigRunFrames{ nullptr, nullptr, nullptr },
	playerBigIdleFrame(nullptr),
	playerBigJumpFrame(nullptr),
	playerBigSlideFrame(nullptr),
	playerBigFlag(nullptr),
	playerMiddleFrame(nullptr),
	_keys(keys),
	_camera(camera)
{}

Player::~Player()
{
	SDL_DestroyTexture(playerSmallIdleFrame);
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(playerSmallRunFrames[i]);
	}
	SDL_DestroyTexture(playerSmallJumpFrame);
	SDL_DestroyTexture(playerSmallSlideFrame);
	SDL_DestroyTexture(playerDeathFrame);
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
	playerDeathFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Death.png");
	playerSmallFlag = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Flag.png");
	playerBigRunFrames[0] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run1.png");
	playerBigRunFrames[1] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run2.png");
	playerBigRunFrames[2] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run3.png");
	playerBigIdleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Idle.png");
	playerBigJumpFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Jump.png");
	playerBigSlideFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Slide.png");
	playerBigFlag = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Flag.png");
	playerMiddleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Middle.png");
	growInfos[0] = { 0.1f, CELL_SIZE * 2, 0, playerSmallIdleFrame };
	growInfos[1] = { 0.2f, CELL_SIZE * 3, CELL_SIZE, playerMiddleFrame };
	growInfos[2] = { 0.3f, CELL_SIZE * 2, 0, playerSmallIdleFrame };
	growInfos[3] = { 0.4f, CELL_SIZE * 3, CELL_SIZE, playerMiddleFrame };
	growInfos[4] = { 0.5f, CELL_SIZE * 4, CELL_SIZE * 2, playerBigIdleFrame };
	growInfos[5] = { 0.6f, CELL_SIZE * 2, 0, playerSmallIdleFrame };
	growInfos[6] = { 0.7f, CELL_SIZE * 3, CELL_SIZE, playerMiddleFrame };
	growInfos[7] = { 1, CELL_SIZE * 4, CELL_SIZE * 2, playerBigIdleFrame };
	flip = SDL_FLIP_NONE;
	type = EntityType::PlayerType;
	center = { rect.w / 2, rect.h / 2 };
	oldPos = { rect.x, rect.y };
	currentState = Alive;
}

void Player::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case Alive:
		if (_keys[SDL_SCANCODE_D])
		{
			die();
		}
		if (_keys[SDL_SCANCODE_G])
		{
			grow();
		}
		Entity::update(deltaTime, collisionManager);
		if (rect.x < _camera.getRect().x)
		{
			rect.x = _camera.getRect().x;
			velocity.x = 0;
		}
		else if (rect.x > _camera.getRect().x + _camera.getRect().w)
		{
			rect.x = (_camera.getRect().x + _camera.getRect().w) - rect.w;
			velocity.x = 0;
		}
		if (isGrounded)
		{
			float frameDelay = velocity.x != 0 ? runAnimationSpeed / abs(velocity.x) : runAnimationSpeed / (80 * deltaTime);
			animation(3, frameDelay, deltaTime);
		}
		previousJumpKeyState = _keys[SDL_SCANCODE_Z];
		if (rect.y > _camera.getRect().y + _camera.getRect().h)
		{
			die();
		}
		break;
	case Growing:
		growTimer += deltaTime;
		for (auto& info : growInfos)
		{
			if (growTimer < info.timeThreshold)
			{
				rect.y = originalY - info.offset;
				rect.h = info.height;
				break;
			}
		}
		if (growTimer >= 1)
		{
			currentState = Alive;
			oldPos.y = originalY - CELL_SIZE * 2;
			isBig = true;
		}
		break;
	case Dying:
		deathWaitTimer += deltaTime;
		if (deathWaitTimer > 0.4f)
		{
			if(_camera.inCamera(rect))
			{
				velocity.y += gravity * deltaTime;
			}
			rect.y += velocity.y;
		}
		break;
	default:
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
	switch (currentState)
	{
	case Alive:
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
			else if (_keys[SDL_SCANCODE_RIGHT] || _keys[SDL_SCANCODE_LEFT] || velocity.x != 0)
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
			else if (_keys[SDL_SCANCODE_RIGHT] || _keys[SDL_SCANCODE_LEFT] || velocity.x != 0)
			{
				SDL_RenderTextureRotated(renderer, playerSmallRunFrames[frameIndex], NULL, &newRect, 0, &center, flip);
			}
			else
			{
				SDL_RenderTextureRotated(renderer, playerSmallIdleFrame, NULL, &newRect, 0, &center, flip);
			}
		}
		break;
	case Growing:
		for (auto& info : growInfos)
		{
			if (growTimer < info.timeThreshold)
			{
				SDL_RenderTextureRotated(renderer, info.texture, NULL, &newRect, 0, &center, flip);
				break;
			}
		}
		break;
	case Dying:
		SDL_RenderTextureRotated(renderer, playerDeathFrame, NULL, &newRect, 0, &center, flip);
		break;
	default:
		break;
	}
}

void Player::restart()
{
	rect = { CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE , CELL_SIZE * 2, CELL_SIZE * 2 };
	life--;
}

void Player::die()
{
	if (canDie)
	{
		deathWaitTimer = 0;
		currentState = Dying;
		velocity.x = 0;
		velocity.y = -8;
	}
}

void Player::grow()
{
	growTimer = 0;
	currentState = Growing;
	frameIndex = 0;
	originalY = rect.y;
}

void Player::onCollisionWithTile(Direction direction)
{
	Entity::onCollisionWithTile(direction);
	switch (direction)
	{
	case Up:
		break;
	case Down:
		isJumping = false;
		break;
	case Left:
		velocity.x = 0;
		break;
	case Right:
		velocity.x = 0;
		break;
	default:
		break;
	}
}

void Player::horizontalMovement(float deltaTime)
{
	isSliding = false;
	maxRunSpeed = (_keys[SDL_SCANCODE_X] && isGrounded) ? 300 : 150;
	runAcceleration = (_keys[SDL_SCANCODE_X] && isGrounded && velocity.x >= 200 * deltaTime) ? 20 : 10;
	if (_keys[SDL_SCANCODE_RIGHT] && velocity.x < maxRunSpeed * deltaTime)
	{
		velocity.x += runAcceleration * deltaTime;
		if (velocity.x > maxRunSpeed * deltaTime) velocity.x = maxRunSpeed * deltaTime;
	}
	if (_keys[SDL_SCANCODE_LEFT] && velocity.x > -maxRunSpeed * deltaTime)
	{
		velocity.x -= runAcceleration * deltaTime;
		if (velocity.x < -maxRunSpeed * deltaTime) velocity.x = -maxRunSpeed * deltaTime;
	}
	if (isGrounded)
	{
		if (_keys[SDL_SCANCODE_LEFT])
		{
			facingRight = false;
		}
		if (_keys[SDL_SCANCODE_RIGHT])
		{
			facingRight = true;
		}
		if (velocity.x > 0 && _keys[SDL_SCANCODE_LEFT])
		{
			isSliding = true;
			velocity.x -= friction * deltaTime;
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		if (_keys[SDL_SCANCODE_RIGHT] && velocity.x > maxRunSpeed * deltaTime)
		{
			velocity.x -= friction * deltaTime;
			if (velocity.x < maxRunSpeed * deltaTime)
			{
				velocity.x = maxRunSpeed * deltaTime;
			}
		}
		if (_keys[SDL_SCANCODE_LEFT] && velocity.x < -maxRunSpeed * deltaTime)
		{
			velocity.x += friction * deltaTime;
			if (velocity.x > -maxRunSpeed * deltaTime)
			{
				velocity.x = -maxRunSpeed * deltaTime;
			}
		}
		if (velocity.x < 0 && _keys[SDL_SCANCODE_RIGHT])
		{
			isSliding = true;
			velocity.x += friction * deltaTime;
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
		if (!_keys[SDL_SCANCODE_RIGHT] && !_keys[SDL_SCANCODE_LEFT])
		{
			if (velocity.x > 0)
			{
				velocity.x -= friction * deltaTime;
				if (velocity.x < 0)
				{
					velocity.x = 0;
				}
			}
			if (velocity.x < 0)
			{
				velocity.x += friction * deltaTime;
				if (velocity.x > 0)
				{
					velocity.x = 0;
				}
			}
		}
	}
	flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

void Player::verticalMovement(float deltaTime)
{
	isFalling = velocity.y > 0;
	if (!previousJumpKeyState && _keys[SDL_SCANCODE_Z] && isGrounded)
	{	
		velocity.y = -jumpForce;
		isJumping = true;
		canSustainJump = true;
		jumpTimer = 0;
	}
	if (!_keys[SDL_SCANCODE_Z] || isFalling || jumpTimer >= 0.2f)
	{
		canSustainJump = false;
	}
	if (_keys[SDL_SCANCODE_Z] && canSustainJump && !isGrounded)
	{
		jumpTimer += deltaTime;
		velocity.y -= jumpHoldForce * deltaTime;
	}
}