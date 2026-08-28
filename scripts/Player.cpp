#include "Player.h"

Player::Player(const std::array<bool, SDL_SCANCODE_COUNT>& keys, float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	_keys(keys),
	smallRunFrames{ nullptr, nullptr, nullptr },
	smallIdleFrame(nullptr),
	smallJumpFrame(nullptr),
	smallSlideFrame(nullptr),
	deathFrame(nullptr),
	smallFlag(nullptr),
	smallShrink(nullptr),
	bigRunFrames{ nullptr, nullptr, nullptr },
	bigIdleFrame(nullptr),
	bigJumpFrame(nullptr),
	bigSlideFrame(nullptr),
	bigFlag(nullptr),
	bigShrink(nullptr),
	middleFrame(nullptr)
{}

Player::~Player()
{
	SDL_DestroyTexture(smallIdleFrame);
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(smallRunFrames[i]);
	}
	SDL_DestroyTexture(smallJumpFrame);
	SDL_DestroyTexture(smallFlag);
	SDL_DestroyTexture(smallShrink);
	SDL_DestroyTexture(smallSlideFrame);
	SDL_DestroyTexture(deathFrame);
	for (int i = 0; i < 3; i++) {
		SDL_DestroyTexture(bigRunFrames[i]);
	}
	SDL_DestroyTexture(bigJumpFrame);
	SDL_DestroyTexture(bigSlideFrame);
	SDL_DestroyTexture(bigFlag);
	SDL_DestroyTexture(bigShrink);
	SDL_DestroyTexture(middleFrame);
}

void Player::init()
{
	rect = { _xPos, _yPos , CELL_SIZE * 2, CELL_SIZE * 2 };
	smallRunFrames[0] = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Run1.png");
	smallRunFrames[1] = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Run2.png");
	smallRunFrames[2] = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Run3.png");
	smallJumpFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Jump.png");
	smallIdleFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Idle.png");
	smallSlideFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Slide.png");
	deathFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Death.png");
	smallFlag = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Flag.png");
	smallShrink = IMG_LoadTexture(_renderer, "assets/Player/Player_Small_Shrink.png");
	bigRunFrames[0] = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Run1.png");
	bigRunFrames[1] = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Run2.png");
	bigRunFrames[2] = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Run3.png");
	bigIdleFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Idle.png");
	bigJumpFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Jump.png");
	bigSlideFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Slide.png");
	bigFlag = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Flag.png");
	bigShrink = IMG_LoadTexture(_renderer, "assets/Player/Player_Big_Shrink.png");
	middleFrame = IMG_LoadTexture(_renderer, "assets/Player/Player_Middle.png");
	growInfos[0] = { 0 , 0.1f, CELL_SIZE * 2, 0, smallIdleFrame };
	growInfos[1] = { 0.1f, 0.2f, CELL_SIZE * 3, -CELL_SIZE, middleFrame };
	growInfos[2] = { 0.2f, 0.3f, CELL_SIZE * 2, 0, smallIdleFrame };
	growInfos[3] = { 0.3f, 0.4f, CELL_SIZE * 3, -CELL_SIZE, middleFrame };
	growInfos[4] = { 0.4f, 0.5f, CELL_SIZE * 4, -CELL_SIZE * 2, bigIdleFrame };
	growInfos[5] = { 0.5f, 0.6f, CELL_SIZE * 2, 0, smallIdleFrame };
	growInfos[6] = { 0.6f, 0.7f, CELL_SIZE * 3, -CELL_SIZE, middleFrame };
	growInfos[7] = { 0.7f, 1, CELL_SIZE * 4, -CELL_SIZE * 2, bigIdleFrame };
	shrinkInfos[0] = { 0 , 1 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[1] = { 2 * shrinkAnimationFrameSeconds , 3 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[2] = { 4 * shrinkAnimationFrameSeconds , 5 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[3] = { 6 * shrinkAnimationFrameSeconds , 7 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[4] = { 8 * shrinkAnimationFrameSeconds , 9 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[5] = { 10 * shrinkAnimationFrameSeconds , 11 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[6] = { 12 * shrinkAnimationFrameSeconds , 13 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[7] = { 14 * shrinkAnimationFrameSeconds , 15 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigJumpFrame };
	shrinkInfos[8] = { 16 * shrinkAnimationFrameSeconds , 17 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[9] = { 18 * shrinkAnimationFrameSeconds , 19 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[10] = { 20 * shrinkAnimationFrameSeconds , 21 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[11] = { 22 * shrinkAnimationFrameSeconds , 23 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[12] = { 24 * shrinkAnimationFrameSeconds , 25 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[13] = { 26 * shrinkAnimationFrameSeconds , 27 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[14] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[15] = { 29 * shrinkAnimationFrameSeconds , 30 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[16] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[17] = { 33 * shrinkAnimationFrameSeconds , 34 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[18] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[19] = { 29 * shrinkAnimationFrameSeconds , 30 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[20] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[21] = { 33 * shrinkAnimationFrameSeconds , 34 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[22] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[23] = { 29 * shrinkAnimationFrameSeconds , 30 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	shrinkInfos[24] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[25] = { 31 * shrinkAnimationFrameSeconds , 32 * shrinkAnimationFrameSeconds, CELL_SIZE * 4, 0, bigShrink };
	shrinkInfos[26] = { 27 * shrinkAnimationFrameSeconds , 28 * shrinkAnimationFrameSeconds, CELL_SIZE * 2, CELL_SIZE * 2, smallShrink };
	flip = SDL_FLIP_NONE;
	type = EntityType::PlayerType;
	center = { rect.w / 2, rect.h / 2 };
	oldCollisionPos = { collisionRect.x, collisionRect.y };
	currentState = PlayerState::Alive;
}

void Player::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case PlayerState::Alive:
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
		else if (rect.x + rect.w > _camera.getRect().x + _camera.getRect().w)
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
	case PlayerState::Growing:
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
			currentState = PlayerState::Alive;
			isBig = true;
			updateCollisionRect();
			oldCollisionPos = { collisionRect.x, collisionRect.y };
		}
		break;
	case PlayerState::Shrinking:
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
			currentState = PlayerState::Alive;
			isBig = false;
			canDie = false;
			invincibleFinishSeconds = 4;
			invincibleTimer = 0;
			invincibleSlowingSeconds = 3;
			invincibleFrameSeconds = 0.016f;
			updateCollisionRect();
			oldCollisionPos = { collisionRect.x, collisionRect.y };
		}
		break;
	case PlayerState::Dying:
		deathWaitTimer += deltaTime;
		if (deathWaitTimer > 0.4f)
		{
			if(_camera.inCamera(rect))
			{
				velocity.y += deathGravity * deltaTime;
			}
			rect.y += velocity.y * deltaTime;
		}
		break;
	default:
		break;
	}
}

void Player::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case PlayerState::Alive:
		Entity::lateUpdate(deltaTime, collisionManager);
		break;
	case PlayerState::Growing:
		break;
	case PlayerState::Shrinking:
		break;
	case PlayerState::Dying:
		break;
	default:
		break;
	}
}

void Player::draw()
{
	if (!_camera.inCamera(rect))
	{
		return;
	}
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	switch (currentState)
	{
	case PlayerState::Alive:
		if (static_cast<int>(invincibleTimer / invincibleFrameSeconds) % 2 == 1) return;
		if (isBig)
		{
			if (isSliding)
			{
				SDL_RenderTextureRotated(_renderer, bigSlideFrame, NULL, &newRect, 0, &center, flip);
			}
			else if (isJumping)
			{
				SDL_RenderTextureRotated(_renderer, bigJumpFrame, NULL, &newRect, 0, &center, flip);
			}
			else if (_keys[SDL_SCANCODE_RIGHT] || _keys[SDL_SCANCODE_LEFT] || velocity.x != 0)
			{
				SDL_RenderTextureRotated(_renderer, bigRunFrames[frameIndex], NULL, &newRect, 0, &center, flip);
			}
			else
			{
				SDL_RenderTextureRotated(_renderer, bigIdleFrame, NULL, &newRect, 0, &center, flip);
			}
		}
		else
		{
			if (isSliding)
			{
				SDL_RenderTextureRotated(_renderer, smallSlideFrame, NULL, &newRect, 0, &center, flip);
			}
			else if (isJumping)
			{
				SDL_RenderTextureRotated(_renderer, smallJumpFrame, NULL, &newRect, 0, &center, flip);
			}
			else if (_keys[SDL_SCANCODE_RIGHT] || _keys[SDL_SCANCODE_LEFT] || velocity.x != 0)
			{
				SDL_RenderTextureRotated(_renderer, smallRunFrames[frameIndex], NULL, &newRect, 0, &center, flip);
			}
			else
			{
				SDL_RenderTextureRotated(_renderer, smallIdleFrame, NULL, &newRect, 0, &center, flip);
			}
		}
		break;
	case PlayerState::Growing:
		for (auto& info : growInfos)
		{
			if (growTimer >= info.startTime && growTimer < info.endTime)
			{
				SDL_RenderTextureRotated(_renderer, info.texture, NULL, &newRect, 0, &center, flip);
				break;
			}
		}
		break;
	case PlayerState::Shrinking:
		for (auto& info : shrinkInfos)
		{
			if (shrinkTimer >= info.startTime && shrinkTimer < info.endTime)
			{
				SDL_RenderTextureRotated(_renderer, info.texture, NULL, &newRect, 0, &center, flip);
				break;
			}
		}
		break;
	case PlayerState::Dying:
		SDL_RenderTextureRotated(_renderer, deathFrame, NULL, &newRect, 0, &center, flip);
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
		currentState = PlayerState::Dying;
		velocity.x = 0;
		if (_camera.inCamera(rect))
		{
			velocity.y = -dieForce;
		}
		else
		{
			velocity.y = 0;
		}
		canDie = false;
	}
}

void Player::grow()
{
	growTimer = 0;
	currentState = PlayerState::Growing;
	originalY = rect.y;
}

void Player::shrink()
{
	shrinkTimer = 0;
	currentState = PlayerState::Shrinking;
	originalY = rect.y;
}

void Player::jump(float force)
{
	velocity.y = -force;
	isJumping = true;
}

void Player::onCollisionWithTile(Direction direction)
{
	Entity::onCollisionWithTile(direction);
	switch (direction)
	{
	case Direction::Up:
		break;
	case Direction::Down:
		isJumping = false;
		break;
	case Direction::Left:
		velocity.x = 0;
		break;
	case Direction::Right:
		velocity.x = 0;
		break;
	default:
		break;
	}
}

void Player::horizontalMovement(float deltaTime)
{
	maxRunSpeed = _keys[SDL_SCANCODE_X] ? maxRunSpeedThreshold : minRunSpeedThreshold;
	if (isGrounded)
	{
		runAcceleration = (_keys[SDL_SCANCODE_X]) ? maxRunAccelerationThreshold : minRunAccelerationThreshold;
	}
	else
	{
		runAcceleration = airAcceleration;
	}
	if (velocity.x == 0)
	{
		isSliding = false;
	}
	if (_keys[SDL_SCANCODE_RIGHT] && velocity.x < maxRunSpeed)
	{
		isSliding = false;
		velocity.x += runAcceleration * deltaTime;
		if (velocity.x > maxRunSpeed) velocity.x = maxRunSpeed;
	}
	if (_keys[SDL_SCANCODE_LEFT] && velocity.x > -maxRunSpeed)
	{
		isSliding = false;
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
		if (velocity.x > 0 && !facingRight)
		{
			isSliding = true;
			velocity.x -= slideFriction * deltaTime;
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0 && facingRight)
		{
			isSliding = true;
			velocity.x += slideFriction * deltaTime;
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
		else if (velocity.x > 0 && isSliding)
		{
			velocity.x -= slideFriction * deltaTime;
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0 && isSliding)
		{
			velocity.x += slideFriction * deltaTime;
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
		else if (_keys[SDL_SCANCODE_RIGHT] && velocity.x > maxRunSpeed)
		{
			velocity.x -= normalFriction * deltaTime;
			if (velocity.x < maxRunSpeed)
			{
				velocity.x = maxRunSpeed;
			}
		}
		else if (_keys[SDL_SCANCODE_LEFT] && velocity.x < -maxRunSpeed)
		{
			velocity.x += normalFriction * deltaTime;
			if (velocity.x > -maxRunSpeed)
			{
				velocity.x = -maxRunSpeed;
			}
		}
		else if (!_keys[SDL_SCANCODE_RIGHT] && !_keys[SDL_SCANCODE_LEFT])
		{
			if (velocity.x > 0)
			{
				velocity.x -= normalFriction * deltaTime;
				if (velocity.x < 0)
				{
					velocity.x = 0;
				}
			}
			else if (velocity.x < 0)
			{
				velocity.x += normalFriction * deltaTime;
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
	bool isFast = abs(velocity.x) > minRunSpeedThreshold;
	initialJumpForce = isFast ? maxJumpForce : minJumpForce;
	isFalling = velocity.y > 0 || !_keys[SDL_SCANCODE_Z];
	if (velocity.y < 0)
	{
		gravity = isFast ? fastJumpGravity : slowJumpGravity;
	}
	if (isFalling)
	{
		gravity = isFast ? fastFallGravity : slowFallGravity;
	}
	if (!previousJumpKeyState && _keys[SDL_SCANCODE_Z] && isGrounded)
	{
		jump(initialJumpForce);
		canSustainJump = true;
	}
	if (isFalling && canSustainJump)
	{
		canSustainJump = false;
	}
}