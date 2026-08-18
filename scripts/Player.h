#include "Entity.h"
#include <array>

class Player : public Entity
{
public:
	Player(const std::array<bool, SDL_SCANCODE_COUNT>& keys);
	~Player();
	void init(SDL_Renderer* renderer) override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void draw(SDL_Renderer* renderer, Camera* camera) override;
	void restart() override;
protected:
	void horizontalMovement(float deltaTime) override;
	void verticalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction) override;
private:
	SDL_Texture* playerSmallRunFrames[3];
	SDL_Texture* playerSmallIdleFrame;
	SDL_Texture* playerSmallJumpFrame;
	SDL_Texture* playerSmallSlideFrame;
	SDL_Texture* playerSmallDeathFrame;
	SDL_Texture* playerSmallFlag;
	SDL_Texture* playerBigRunFrames[3];
	SDL_Texture* playerBigIdleFrame;
	SDL_Texture* playerBigJumpFrame;
	SDL_Texture* playerBigSlideFrame;
	SDL_Texture* playerBigFlag;
	SDL_Texture* playerMiddleFrame;
	const std::array<bool, SDL_SCANCODE_COUNT>& _keys;
	float maxRunSpeed;
	float runAcceleration;
	float runAnimationSpeed = 0.15f;
	bool previousJumpKeyState = false;
	bool isFalling = false;
	bool isJumping = false;
	bool canSustainJump = false;
	bool isSliding = false;
	bool isDied = false;
	bool canDie = true;
	bool starMode = false;
	bool facingRight = true;
	float jumpTimer = 0;
	float jumpForce = 4;
	float jumpHoldForce = 35;
	bool isBig = false;
	int invisibleAnimation = 0;
	int growAnimation = 0;
	float originalY;
	int life = 3;
};