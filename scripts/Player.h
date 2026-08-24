#include "Entity.h"
#include <array>

enum PlayerState
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
	Player(const std::array<bool, SDL_SCANCODE_COUNT>& keys, Camera& camera);
	~Player();
	void init(SDL_Renderer* renderer) override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void draw(SDL_Renderer* renderer, Camera* camera) override;
	void restart() override;
	void die();
	void grow();
	void shrink();
protected:
	void horizontalMovement(float deltaTime) override;
	void verticalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction) override;
private:
	SDL_Texture* playerSmallRunFrames[3];
	SDL_Texture* playerSmallIdleFrame;
	SDL_Texture* playerSmallJumpFrame;
	SDL_Texture* playerSmallSlideFrame;
	SDL_Texture* playerDeathFrame;
	SDL_Texture* playerSmallFlag;
	SDL_Texture* playerSmallShrink;
	SDL_Texture* playerBigRunFrames[3];
	SDL_Texture* playerBigIdleFrame;
	SDL_Texture* playerBigJumpFrame;
	SDL_Texture* playerBigSlideFrame;
	SDL_Texture* playerBigFlag;
	SDL_Texture* playerBigShrink;
	SDL_Texture* playerMiddleFrame;
	const std::array<bool, SDL_SCANCODE_COUNT>& _keys;
	Camera& _camera;
	PlayerState currentState = Alive;
	float maxRunSpeed = 150;
	const float minRunSpeedThreshold = 150;
	const float maxRunSpeedThreshold = 300;
	const float runAcceleration = 500;
	const float slideFriction = 320;
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
	const float jumpGravity = 500;
	const float fallGravity = 1500;
	float initialJumpForce = 365;
	const float minJumpForce = 365;
	const float maxJumpForce = 380;
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