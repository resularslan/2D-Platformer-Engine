#include "Entity.h"
#include <array>

class Player : public Entity
{
public:
	Player(std::array<bool, SDL_SCANCODE_COUNT>& keys);
	~Player();
	void init(SDL_Renderer* renderer) override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void draw(SDL_Renderer* renderer, Camera* camera) override;
	void restart() override;
protected:
	void movement(float deltaTime) override;
	void onCollisionWithEntity(EntityType type, Direction direction) override;
private:
	void horizontalMovement(float deltaTime);
	void verticalMovement(float deltaTime);
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
	std::array<bool, SDL_SCANCODE_COUNT>& _keys;
	float maxRunSpeed;
	float runAcceleration;
	float runAnimationSpeed;
	float oldVelocityX;
	bool isFalling = false;
	bool isJumping = false;
	bool isSliding = false;
	bool isDied = false;
	bool canDie = true;
	bool starMode = false;
	bool facingRight = true;
	float jumpTime = 0;
	bool isBig = false;
	int invisibleAnimation = 0;
	int growAnimation = 0;
	float originalY;
	int life = 3;
};