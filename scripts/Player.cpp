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
	playerSmallShrink(nullptr),
	playerBigRunFrames{ nullptr, nullptr, nullptr },
	playerBigIdleFrame(nullptr),
	playerBigJumpFrame(nullptr),
	playerBigSlideFrame(nullptr),
	playerBigFlag(nullptr),
	playerBigShrink(nullptr),
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
	SDL_DestroyTexture(playerSmallFlag);
	SDL_DestroyTexture(playerSmallShrink);
	SDL_DestroyTexture(playerSmallSlideFrame);
	SDL_DestroyTexture(playerDeathFrame);
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(playerBigRunFrames[i]);
	}
	SDL_DestroyTexture(playerBigJumpFrame);
	SDL_DestroyTexture(playerBigSlideFrame);
	SDL_DestroyTexture(playerBigFlag);
	SDL_DestroyTexture(playerBigShrink);
	SDL_DestroyTexture(playerMiddleFrame);
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
	playerSmallShrink = IMG_LoadTexture(renderer, "assets/Player/Player_Small_Shrink.png");
	playerBigRunFrames[0] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run1.png");
	playerBigRunFrames[1] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run2.png");
	playerBigRunFrames[2] = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Run3.png");
	playerBigIdleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Idle.png");
	playerBigJumpFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Jump.png");
	playerBigSlideFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Slide.png");
	playerBigFlag = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Flag.png");
	playerBigShrink = IMG_LoadTexture(renderer, "assets/Player/Player_Big_Shrink.png");
	playerMiddleFrame = IMG_LoadTexture(renderer, "assets/Player/Player_Middle.png");
	growInfos[0] = { 0 , 0.1f, CELL_SIZE * 2, 0, playerSmallIdleFrame };
	growInfos[1] = { 0.1f, 0.2f, CELL_SIZE * 3, -CELL_SIZE, playerMiddleFrame };
	growInfos[2] = { 0.2f, 0.3f, CELL_SIZE * 2, 0, playerSmallIdleFrame };
	growInfos[3] = { 0.3f, 0.4f, CELL_SIZE * 3, -CELL_SIZE, playerMiddleFrame };
	growInfos[4] = { 0.4f, 0.5f, CELL_SIZE * 4, -CELL_SIZE * 2, playerBigIdleFrame };
	growInfos[5] = { 0.5f, 0.6f, CELL_SIZE * 2, 0, playerSmallIdleFrame };
	growInfos[6] = { 0.6f, 0.7f, CELL_SIZE * 3, -CELL_SIZE, playerMiddleFrame };
	growInfos[7] = { 0.7f, 1, CELL_SIZE * 4, -CELL_SIZE * 2, playerBigIdleFrame };
	shrinkInfos[0] = { 0 , 1 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[1] = { 2 * shrinkAnimationFrameSeconds , 3 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[2] = { 4 * shrinkAnimationFrameSeconds , 5 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[3] = { 6 * shrinkAnimationFrameSeconds , 7 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[4] = { 8 * shrinkAnimationFrameSeconds , 9 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[5] = { 10 * shrinkAnimationFrameSeconds , 11 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[6] = { 12 * shrinkAnimationFrameSeconds , 13 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[7] = { 14 * shrinkAnimationFrameSeconds , 15 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigJumpFrame };
	shrinkInfos[8] = { 16 * shrinkAnimationFrameSeconds , 17 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[9] = { 18 * shrinkAnimationFrameSeconds , 19 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[10] = { 20 * shrinkAnimationFrameSeconds , 21 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[11] = { 22 * shrinkAnimationFrameSeconds , 23 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[12] = { 24 * shrinkAnimationFrameSeconds , 25 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[13] = { 26 * shrinkAnimationFrameSeconds , 27 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[14] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[15] = { 29 * shrinkAnimationFrameSeconds , 30 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[16] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[17] = { 33 * shrinkAnimationFrameSeconds , 34 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[18] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[19] = { 29 * shrinkAnimationFrameSeconds , 30 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[20] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[21] = { 33 * shrinkAnimationFrameSeconds , 34 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[22] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[23] = { 29 * shrinkAnimationFrameSeconds , 30 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	shrinkInfos[24] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[25] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, playerBigShrink };
	shrinkInfos[26] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, playerSmallShrink };
	flip = SDL_FLIP_NONE;
	type = EntityType::PlayerType;
	center = { rect.w / 2, rect.h / 2 };
	oldCollisionPos = { collisionRect.x, collisionRect.y };
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
		if (_keys[SDL_SCANCODE_S])
		{
			shrink();
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
			float frameDelay = velocity.x != 0 ? runAnimationSpeed / abs(velocity.x) : runAnimationSpeed / minAnimationRunSpeed;
			animation(3, frameDelay, deltaTime);
		}
		previousJumpKeyState = _keys[SDL_SCANCODE_Z];
		if (rect.y > _camera.getRect().y + _camera.getRect().h)
		{
			die();
		}
		if (invincibleTimer < invincibleFinishSeconds)
		{
			invincibleTimer += deltaTime;
		}
		else
		{
			invincibleSlowingSeconds = 0;
			invincibleFinishSeconds = 0;
			invincibleTimer = 0;
			canDie = true;
		}
		if (invincibleTimer > invincibleSlowingSeconds)
		{
			invincibleFrameSeconds += 0.4f * deltaTime;
		}
		break;
	case Growing:
		growTimer += deltaTime;
		for (auto& info : growInfos)
		{
			if (growTimer >= info.startTime && growTimer < info.endTime)
			{
				rect.y = originalY + info.offset;
				rect.h = info.height;
				break;
			}
		}
		if (growTimer >= 1)
		{
			currentState = Alive;
			isBig = true;
			updateCollisionRect();
		}
		break;
	case Shrinking:
		shrinkTimer += deltaTime;
		for (auto& info : shrinkInfos)
		{
			if (shrinkTimer >= info.startTime && shrinkTimer < info.endTime)
			{
				rect.y = originalY + info.offset;
				rect.h = info.height;
				break;
			}
		}
		if (shrinkTimer >= 29 * shrinkAnimationFrameSeconds)
		{
			currentState = Alive;
			isBig = false;
			canDie = false;
			invincibleFinishSeconds = 4;
			invincibleTimer = 0;
			invincibleSlowingSeconds = 3;
			invincibleFrameSeconds = 0.016f;
			updateCollisionRect();
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
		if (static_cast<int>(invincibleTimer / invincibleFrameSeconds) % 2 == 1) return;
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
			if (growTimer >= info.startTime && growTimer < info.endTime)
			{
				SDL_RenderTextureRotated(renderer, info.texture, NULL, &newRect, 0, &center, flip);
				break;
			}
		}
		break;
	case Shrinking:
		for (auto& info : shrinkInfos)
		{
			if (shrinkTimer >= info.startTime && shrinkTimer < info.endTime)
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
		if (isBig)
		{
			rect.h = CELL_SIZE * 2;
			rect.y += CELL_SIZE * 3;
		}
		else
		{
			rect.y += CELL_SIZE / 2;
		}
		deathWaitTimer = 0;
		currentState = Dying;
		velocity.x = 0;
		if (_camera.inCamera(rect))
		{
			velocity.y = -8;
		}
		else
		{
			velocity.y = 0;
		}
	}
}

void Player::grow()
{
	growTimer = 0;
	currentState = Growing;
	originalY = rect.y;
}

void Player::shrink()
{
	shrinkTimer = 0;
	currentState = Shrinking;
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
	maxRunSpeed = (_keys[SDL_SCANCODE_X] && isGrounded) ? maxRunSpeedThreshold : minRunSpeedThreshold;
	if (_keys[SDL_SCANCODE_RIGHT] && velocity.x < maxRunSpeed)
	{
		velocity.x += runAcceleration * deltaTime;
		if (velocity.x > maxRunSpeed) velocity.x = maxRunSpeed;
	}
	if (_keys[SDL_SCANCODE_LEFT] && velocity.x > -maxRunSpeed)
	{
		velocity.x -= runAcceleration * deltaTime;
		if (velocity.x < -maxRunSpeed) velocity.x = -maxRunSpeed;
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
			velocity.x -= slideFriction * deltaTime;
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		if (_keys[SDL_SCANCODE_RIGHT] && velocity.x > maxRunSpeed)
		{
			velocity.x -= friction * deltaTime;
			if (velocity.x < maxRunSpeed)
			{
				velocity.x = maxRunSpeed;
			}
		}
		if (_keys[SDL_SCANCODE_LEFT] && velocity.x < -maxRunSpeed)
		{
			velocity.x += friction * deltaTime;
			if (velocity.x > -maxRunSpeed)
			{
				velocity.x = -maxRunSpeed;
			}
		}
		if (velocity.x < 0 && _keys[SDL_SCANCODE_RIGHT])
		{
			isSliding = true;
			velocity.x += slideFriction * deltaTime;
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
	initialJumpForce = velocity.x > minRunSpeedThreshold ? maxJumpForce : minJumpForce;
	isFalling = velocity.y > 0;
	if (!previousJumpKeyState && _keys[SDL_SCANCODE_Z] && isGrounded)
	{	
		gravity = jumpGravity;
		velocity.y = -initialJumpForce;
		isJumping = true;
		canSustainJump = true;
	}
	if ((!_keys[SDL_SCANCODE_Z] || isFalling) && canSustainJump)
	{
		gravity = fallGravity;
		canSustainJump = false;
	}
}