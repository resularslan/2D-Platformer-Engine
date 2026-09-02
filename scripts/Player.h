#pragma once

#include "Entity.h"
#include <array>

enum class PlayerState
{
	Alive,
	Growing,
	Shrinking,
	Dying,
};

struct TransformationAnimationInfo
{
	float startTime;
	float endTime;
	float height;
	float offset;
	SDL_Texture* texture;
};

class Player : public Entity
{
public:
	Player(const std::array<bool, SDL_SCANCODE_COUNT>& keys, float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id);
	~Player();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
	void die() override;
	void takeDamage();
	void grow();
	void shrink();
	void jump(float force);
protected:
	void horizontalMovement(float deltaTime) override;
	void verticalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction) override;
private:
	SDL_Texture* smallRunFrames[3];
	SDL_Texture* smallIdleFrame;
	SDL_Texture* smallJumpFrame;
	SDL_Texture* smallSlideFrame;
	SDL_Texture* deathFrame;
	SDL_Texture* smallFlag;
	SDL_Texture* smallShrink;
	SDL_Texture* bigRunFrames[3];
	SDL_Texture* bigIdleFrame;
	SDL_Texture* bigJumpFrame;
	SDL_Texture* bigSlideFrame;
	SDL_Texture* bigFlag;
	SDL_Texture* bigShrink;
	SDL_Texture* middleFrame;
	const std::array<bool, SDL_SCANCODE_COUNT>& _keys;
	PlayerState currentState = PlayerState::Alive;
	float maxRunSpeed = 150;
	const float minRunSpeedThreshold = 150;
	const float maxRunSpeedThreshold = 300;
	float runAcceleration = 350;
	const float minRunAccelerationThreshold = 300;
	const float maxRunAccelerationThreshold = 487;
	const float airAcceleration = 300;
	const float normalFriction = 300;
	const float slideFriction = 600;
	const float minAnimationRunSpeed = 80;
	const float runAnimationSpeed = 10;
	bool previousJumpKeyState = false;
	bool isFalling = false;
	bool isJumping = false;
	bool canSustainJump = false;
	bool isSliding = false;
	bool canDie = true;
	bool starMode = false;
	bool facingRight = true;
	const float slowJumpGravity = 700;
	const float fastJumpGravity = 873;
	const float slowFallGravity = 2456;
	const float fastFallGravity = 3063;
	float initialJumpForce = 470;
	const float minJumpForce = 470;
	const float maxJumpForce = 560;
	const float dieForce = 390;
	const float deathGravity = 900;
	bool isBig = false;
	float deathWaitTimer = 0;
	float invincibleTimer = 0;
	float invincibleFinishSeconds = 0;
	float invincibleSlowingSeconds = 0;
	float invincibleFrameSeconds = 0;
	TransformationAnimationInfo growInfos[8];
	float growTimer = 0;
	TransformationAnimationInfo shrinkInfos[27];
	const float shrinkAnimationFrameSeconds = 0.016f;
	float shrinkTimer = 0;
	float originalY = 0;
	int life = 3;
};