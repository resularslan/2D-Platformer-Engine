#include "Entity.h"

class Player : public Entity
{
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
	SDL_FlipMode flip;
	float oldvelocity_x;
	int frameIndex = 0;
	int animFrameDelay;
	Uint32 lastFrameTime = 0;
	bool grounded;
	bool falling;
	bool jumping;
	bool sliding;
	bool facingRight = true;
	bool isDied = false;
	bool canDie = true;
	bool starMode = false;
	float jumpTime = 0;
	bool isBig = false;
	int invisibleAnimation = 0;
	int growAnimation = -1;
	float originalY;
	int life = 3;
protected:
	void movement(float deltaTime) override;
public:
	void init() override;
	void update(float deltaTime) override;
	void restart() override;
	void draw(SDL_Renderer* renderer) override;
	~Player();
};