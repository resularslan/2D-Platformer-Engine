#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <fstream>

using namespace std;

#define CELL_SIZE 16
#define WIDTH_CELL 424
#define HEIGHT_CELL 30
#define WINDOW_WIDTH (32 * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT_CELL * CELL_SIZE)
#define CONST (CELL_SIZE / 8)

struct vector2
{
    float x;
    float y;
    vector2 normalized()
    {
        return { (float) (x > 0) - (x < 0), (float) (y > 0) - (y < 0) };
    }
};

struct Enemy
{
	SDL_FRect rect;
	vector2 velocity = { 0.75f, 0};
	vector2 oldpos;
	vector2 direction = { -1, 0 };
	bool isDiedByPlayer = false;
	bool isDiedByStar = false;
	bool active = true;
	int frameIndex = 0;
	char random = rand() % 2;
	Uint32 lastFrameTime = 0;
	void restart()
	{
		velocity = { 0.75f, 0 };
		isDiedByPlayer = false;
		isDiedByStar = false;
		active = true;
		direction = { -1, 0 };
	}
};

struct Mushroom
{
	SDL_FRect rect;
	vector2 velocity = { 0.75f, 0 };
	vector2 oldpos;
	vector2 direction = { 1, 0 };
	int animTimer = 0;
	bool active = false;
	void restart()
	{
		velocity = { 0.75f, 0 };
		animTimer = 0;
		active = false;
		direction = { 1, 0 };
	}
};

struct Star
{
	SDL_FRect rect;
	SDL_FRect srcRect;
	vector2 velocity = { 0.75f, 0 };
	vector2 oldpos;
	vector2 direction = { 1, 0 };
	int animTimer = 0;
	int frameIndex = 0;
	Uint32 lastFrameTime = 0;
	bool active = false;
	void restart()
	{
		velocity = { 0.75f, 0 };
		animTimer = 0;
		active = false;
		direction = { 1, 0 };
	}
};

struct Player
{
	SDL_FRect rect;
	SDL_FlipMode flip;
	vector2 velocity;
	vector2 oldpos;
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
	bool flagAnimation = false;
	float originalY;
	int life = 3;
	void restart()
	{
		facingRight = true;
		flagAnimation = false;
		velocity = { 0 , 0 };
		canDie = true;
		jumpTime = 0;
		isBig = false;
		invisibleAnimation = 0;
		originalY = 0;
		isDied = false;
	}
};

struct Tile
{
	int tilemap;
	SDL_FRect destRect;
	float originalY;
	int bounceTimer;
	bool isCollided = false;
	bool anim = false;
	bool active = true;
	void restart()
	{
		isCollided = false;
		anim = false;
		active = true;
	}
};

struct Coin
{
	SDL_FRect destRect;
	SDL_FRect srcRect;
	int number = 8;
	int animTime;
	int frameIndex = 0;
	Uint32 lastFrameTime = 0;
	bool active = false;
	void restart()
	{
		number = 8;
		active = false;
	}
};

bool running = true;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* tileset_texture;
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
SDL_Texture* firstEnemyWalkFrames[2];
SDL_Texture* firstEnemyPlayerKilledFrame;
SDL_Texture* firstEnemyDeathFrame;
SDL_Texture* secondEnemyWalkFrames[2];
SDL_Texture* secondEnemyPlayerKilledFrame;
SDL_Texture* secondEnemyDeathFrame;
SDL_Texture* growMushroomTexture;
SDL_Texture* healthMushroomTexture;
SDL_Texture* miscTexture;
SDL_FRect camera;
Player player;
Enemy firstEnemies[16];
Enemy secondEnemies[1];
Coin coin;
Mushroom growMushroom;
Mushroom healthMushroom;
Star star;
Tile tile[HEIGHT_CELL][WIDTH_CELL];
Tile collisiontiles[HEIGHT_CELL / 2][WIDTH_CELL / 2];
SDL_FPoint entityCenter = { CELL_SIZE, CELL_SIZE };
SDL_FPoint center = { CELL_SIZE / 2, CELL_SIZE / 2 };
//Uint64 player.lastFrameTime = SDL_GetPerformanceCounter();
//float deltaTime;

bool keys[SDL_SCANCODE_COUNT] = { false };
bool tmpKey;

float acceleration = 0.1f * CONST;
float maxSpeed;
const float friction = 0.04f * CONST;
const float gravity = 0.2f * CONST;

bool stopMovement = false;

int enemyAnimFrameDelay = 100;
Uint32 currentTime = (Uint32)SDL_GetTicks();

// Brick pieces
int angle;
SDL_FRect srect1 = { 34, 272, 8, 8 };
SDL_FRect srect2 = { 43, 272, 8, 8 };
SDL_FRect srect3 = { 34, 281, 8, 8 };
SDL_FRect srect4 = { 43, 281, 8, 8 };

const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint64 frameStart;
int frameTime;

void initialize();
void loadTilemap();
void loadCollision(bool restart);
void updateCamera();
void renderTexture(SDL_Texture* texture, SDL_FRect* srcRect, SDL_FRect* rect, double angle, SDL_FPoint* center, SDL_FlipMode flip);
void renderTilemap();
void getOldVectors();
void renderEnemies();
void handlePlayerEnemyCollision(Player* player, Enemy* enemy, vector2* oldplayerpos, vector2* oldenemypos);
void handleEnemyEnemyCollision(Enemy* enemy, Enemy* otherEnemy, vector2* oldpos, vector2* oldOtherEnemyPos);
void handleTileCollision(SDL_FRect* entity, vector2* oldPos);
void handleEvents();
void horizontalMovement();
void veriticalMovement();
void playerAnimation();
void restart();
void renderStar();
void itemMovement();
void rendergrowMushrooms();
void renderhealthMushrooms();
void enemyMovement();
void coinAnimation();
void updateBrick();
void update();
void destroyEverything();

int main()
{
	initialize();
	while (running)
	{
		frameStart = SDL_GetTicks();
		/*deltaTime = (SDL_GetPerformanceCounter() - lastFrameTime) / (float)SDL_GetPerformanceFrequency();
		lastFrameTime = SDL_GetPerformanceCounter();*/
		update();
		frameTime = (int) (SDL_GetTicks() - frameStart);
		if (frameTime < frameDelay)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	destroyEverything();
	return 0;
}

void update()
{
	currentTime = (Uint32)SDL_GetTicks();
	if (!stopMovement)
	{
		if (!player.flagAnimation)
		{
			handleEvents();
		}
		enemyMovement();
		itemMovement();
		getOldVectors();
		horizontalMovement();
		veriticalMovement();
	}
	SDL_SetRenderDrawColor(renderer, 92, 148, 252, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	renderTilemap();
	coinAnimation();
	renderEnemies();
	updateBrick();
	playerAnimation();
	SDL_RenderPresent(renderer);
	updateCamera();
}

void updateCamera()
{
	if (player.rect.x > camera.x + (camera.w / 10 * 4))
	{
		camera.x = player.rect.x - (camera.w / 10 * 4);
	}
	if (camera.x < 0) camera.x = 0;
	else if (camera.x + camera.w > WIDTH_CELL * CELL_SIZE) camera.x = WIDTH_CELL * CELL_SIZE - camera.w;
}

void renderTexture(SDL_Texture* texture, SDL_FRect* srcRect, SDL_FRect* rect, double angle, SDL_FPoint* center, SDL_FlipMode flip)
{
	SDL_FRect newRect = {
		roundf(rect->x - camera.x),
		roundf(rect->y - camera.y),
		rect->w,
		rect->h
	};
	SDL_RenderTextureRotated(renderer, texture, srcRect, &newRect, angle, center, flip);
}

void getOldVectors()
{
	player.oldpos.x = player.rect.x;
	player.oldpos.y = player.rect.y;
	player.oldvelocity_x = player.velocity.x;
	for (Enemy& firstEnemy : firstEnemies)
	{
		firstEnemy.oldpos.x = firstEnemy.rect.x;
		firstEnemy.oldpos.y = firstEnemy.rect.y;
	}
	for (Enemy& secondEnemy : secondEnemies)
	{
		secondEnemy.oldpos.x = secondEnemy.rect.x;
		secondEnemy.oldpos.y = secondEnemy.rect.y;
	}
	growMushroom.oldpos.x = growMushroom.rect.x;
	growMushroom.oldpos.y = growMushroom.rect.y;
	healthMushroom.oldpos.x = healthMushroom.rect.x;
	healthMushroom.oldpos.y = healthMushroom.rect.y;
	star.oldpos.x = star.rect.x;
	star.oldpos.y = star.rect.y;
}

void enemyMovement()
{
	for (Enemy& firstEnemy : firstEnemies)
	{
		if (firstEnemy.rect.x <= camera.x + camera.w && firstEnemy.rect.x + firstEnemy.rect.w >= camera.x - 8 * CELL_SIZE 
			&& firstEnemy.rect.y <= camera.y + camera.h + 8 * CELL_SIZE && !firstEnemy.isDiedByPlayer && firstEnemy.active)
		{
			firstEnemy.velocity.y += gravity;
			firstEnemy.rect.y += firstEnemy.velocity.y;
			firstEnemy.rect.x += firstEnemy.velocity.x * firstEnemy.direction.normalized().x * CONST;
			for (Enemy& otherfirstEnemy : firstEnemies)
			{
				if (&otherfirstEnemy != &firstEnemy && !firstEnemy.isDiedByPlayer && !otherfirstEnemy.isDiedByPlayer && otherfirstEnemy.active)
				{
					handleEnemyEnemyCollision(&firstEnemy, &otherfirstEnemy, &firstEnemy.oldpos, &otherfirstEnemy.oldpos);
				}
			}
			for (Enemy& secondEnemy : secondEnemies)
			{
				if (!firstEnemy.isDiedByStar && !firstEnemy.isDiedByPlayer && secondEnemy.active)
				{
					handleEnemyEnemyCollision(&firstEnemy, &secondEnemy, &firstEnemy.oldpos, &secondEnemy.oldpos);
				}
			}
			if (firstEnemy.isDiedByStar)
			{
				firstEnemy.velocity.x = 1.5f;
				if (firstEnemy.random == 1) firstEnemy.direction.x = 1;
				else
				{
					firstEnemy.direction.x = -1;
				}
			}
			if (!player.isDied && !firstEnemy.isDiedByStar)
			{
				handlePlayerEnemyCollision(&player, &firstEnemy, &player.oldpos, &firstEnemy.oldpos);
			}
			if (!firstEnemy.isDiedByStar) handleTileCollision(&firstEnemy.rect, &firstEnemy.oldpos);
		}
	}
	for (Enemy& secondEnemy : secondEnemies)
	{
		if (secondEnemy.rect.x <= camera.x + camera.w && secondEnemy.rect.x + secondEnemy.rect.w >= camera.x - 8 * CELL_SIZE 
			&& secondEnemy.rect.y <= camera.y + camera.h + 8 * CELL_SIZE && secondEnemy.active)
		{
			secondEnemy.velocity.y += gravity;
			secondEnemy.rect.y += secondEnemy.velocity.y;
			secondEnemy.rect.x += secondEnemy.velocity.x * secondEnemy.direction.normalized().x * CONST;
			for (Enemy& othersecondEnemy : secondEnemies)
			{
				if (&othersecondEnemy != &secondEnemy && !othersecondEnemy.isDiedByStar && othersecondEnemy.active)
				{
					handleEnemyEnemyCollision(&secondEnemy, &othersecondEnemy, &secondEnemy.oldpos, &othersecondEnemy.oldpos);
				}
			}
			for (Enemy& firstEnemy : firstEnemies)
			{
				if (!firstEnemy.isDiedByStar && !firstEnemy.isDiedByPlayer && secondEnemy.active)
				{
					handleEnemyEnemyCollision(&secondEnemy, &firstEnemy, &secondEnemy.oldpos, &firstEnemy.oldpos);
				}
			}
			if (secondEnemy.isDiedByStar)
			{
				secondEnemy.velocity.x = 1.5f;
				if (secondEnemy.random == 1) secondEnemy.direction.x = 1;
				else
				{
					secondEnemy.direction.x = -1;
				}
			}
			if (secondEnemy.isDiedByPlayer && abs(secondEnemy.velocity.x) == 0.75f)
			{
				secondEnemy.velocity.x = 0;
			}
			else if (!secondEnemy.isDiedByPlayer)
			{
				secondEnemy.velocity.x = 0.75f;
			}
			else if (secondEnemy.isDiedByPlayer && abs(secondEnemy.velocity.x) == 3)
			{
				if (secondEnemy.rect.x >= camera.x + camera.w || secondEnemy.rect.x + secondEnemy.rect.w <= camera.x) secondEnemy.active = false;
			}
			if (!player.isDied && !secondEnemy.isDiedByStar)
			{
				handlePlayerEnemyCollision(&player, &secondEnemy, &player.oldpos, &secondEnemy.oldpos);
			}
			if (!secondEnemy.isDiedByStar) handleTileCollision(&secondEnemy.rect, &secondEnemy.oldpos);
		}
	}
}

void renderEnemies()
{
	for (Enemy& firstEnemy : firstEnemies)
	{
		if (firstEnemy.rect.x <= camera.x + camera.w && firstEnemy.rect.x + firstEnemy.rect.w >= camera.x &&
			firstEnemy.rect.y <= camera.y + camera.h && firstEnemy.rect.y + firstEnemy.rect.h >= camera.y && firstEnemy.active)
		{
			if (!firstEnemy.isDiedByStar && !firstEnemy.isDiedByPlayer)
			{
				if (currentTime > firstEnemy.lastFrameTime + enemyAnimFrameDelay && !stopMovement)
				{
					firstEnemy.lastFrameTime = currentTime;
					firstEnemy.frameIndex = (firstEnemy.frameIndex + 1) % 2;
				}
				renderTexture(firstEnemyWalkFrames[firstEnemy.frameIndex], NULL, &firstEnemy.rect, 0, &entityCenter, SDL_FLIP_NONE);
			}
			else if (firstEnemy.isDiedByPlayer)
			{
				if (currentTime < firstEnemy.lastFrameTime + 400)
				{
					renderTexture(firstEnemyPlayerKilledFrame, NULL, &firstEnemy.rect, 0, &entityCenter, SDL_FLIP_NONE);
				}
				else
				{
					firstEnemy.active = false;
				}
			}
			else if (firstEnemy.isDiedByStar)
			{
				renderTexture(firstEnemyDeathFrame, NULL, &firstEnemy.rect, 0, &entityCenter, SDL_FLIP_NONE);
			}
		}
	}
	for (Enemy& secondEnemy : secondEnemies)
	{
		if (secondEnemy.rect.x <= camera.x + camera.w && secondEnemy.rect.x + secondEnemy.rect.w >= camera.x &&
			secondEnemy.rect.y <= camera.y + camera.h && secondEnemy.rect.y + secondEnemy.rect.h >= camera.y && secondEnemy.active)
		{
			SDL_FlipMode flip = secondEnemy.direction.x < 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			if (!secondEnemy.isDiedByStar && !secondEnemy.isDiedByPlayer)
			{
				if (currentTime > secondEnemy.lastFrameTime + enemyAnimFrameDelay && !stopMovement)
				{
					secondEnemy.lastFrameTime = currentTime;
					secondEnemy.frameIndex = (secondEnemy.frameIndex + 1) % 2;
				}
				renderTexture(secondEnemyWalkFrames[secondEnemy.frameIndex], NULL, &secondEnemy.rect, 0, &entityCenter, flip);
			}
			else if (secondEnemy.isDiedByPlayer)
			{
				renderTexture(secondEnemyPlayerKilledFrame, NULL, &secondEnemy.rect, 0, &entityCenter, flip);
				if (currentTime > secondEnemy.lastFrameTime + 5000 && secondEnemy.velocity.x == 0)
				{
					secondEnemy.isDiedByPlayer = false;
				}
			}
			else if (secondEnemy.isDiedByStar)
			{
				renderTexture(secondEnemyDeathFrame, NULL, &secondEnemy.rect, 0, &entityCenter, flip);
			}
		}
	}
}

void itemMovement()
{	
	if (growMushroom.active && growMushroom.rect.x <= camera.x + camera.w && growMushroom.rect.x + growMushroom.rect.w >= camera.x &&
		growMushroom.rect.y <= camera.y + camera.h + 8 * CELL_SIZE && !stopMovement)
	{
		if (growMushroom.animTimer > 0)
		{
			growMushroom.rect.y -= 1 * CONST;
			growMushroom.animTimer--;
		}
		else
		{
			growMushroom.velocity.y += gravity;
			growMushroom.rect.y += growMushroom.velocity.y;
			growMushroom.rect.x += growMushroom.velocity.x * growMushroom.direction.normalized().x * CONST;
			handleTileCollision(&growMushroom.rect, &growMushroom.oldpos);
		}
		if (SDL_HasRectIntersectionFloat(&growMushroom.rect, &player.rect))
		{
			growMushroom.active = false;
			if (!player.isBig) player.growAnimation = 45;
			player.originalY = player.rect.y;
		}
	}
	else
	{
		growMushroom.direction.x = 1;
		growMushroom.active = false;
	}
	if (healthMushroom.active && healthMushroom.rect.x <= camera.x + camera.w && healthMushroom.rect.x + healthMushroom.rect.w >= camera.x &&
		healthMushroom.rect.y <= camera.y + camera.h + 8 * CELL_SIZE && !stopMovement)
	{
		if (healthMushroom.animTimer > 0)
		{
			healthMushroom.rect.y -= 1 * CONST;
			healthMushroom.animTimer--;
		}
		else
		{
			healthMushroom.velocity.y += gravity;
			healthMushroom.rect.y += healthMushroom.velocity.y;
			healthMushroom.rect.x += healthMushroom.velocity.x * healthMushroom.direction.normalized().x * CONST;
			handleTileCollision(&healthMushroom.rect, &healthMushroom.oldpos);
		}
		if (SDL_HasRectIntersectionFloat(&healthMushroom.rect, &player.rect))
		{
			healthMushroom.active = false;
			player.life++;
		}
	}
	else
	{
		healthMushroom.direction.x = 1;
		healthMushroom.active = false;
	}
	if (star.active && star.rect.x <= camera.x + camera.w && star.rect.x + star.rect.w >= camera.x &&
		star.rect.y <= camera.y + camera.h + 8 * CELL_SIZE && !stopMovement)
	{
		if (star.animTimer > 0)
		{
			star.rect.y -= 1 * CONST;
			star.animTimer--;
		}
		else
		{
			star.velocity.y += gravity;
			star.rect.y += star.velocity.y;
			star.rect.x += star.velocity.x * star.direction.normalized().x * CONST;
			handleTileCollision(&star.rect, &star.oldpos);
		}
		if (SDL_HasRectIntersectionFloat(&star.rect, &player.rect))
		{
			star.active = false;
			player.invisibleAnimation = 500;
			player.starMode = true;
		}
	}
	else
	{
		star.direction.x = 1;
		star.active = false;
	}
}

void coinAnimation()
{
	if (currentTime > coin.lastFrameTime + 10 && !stopMovement)
	{
		coin.lastFrameTime = currentTime;
		coin.frameIndex = (coin.frameIndex + 1) % 4;
	}
	coin.srcRect = { coin.frameIndex * 17.0f, 17, 16, 16 };
	if (coin.animTime > 10) {
		coin.destRect.y -= 7 * CONST;
	}
	else {
		coin.destRect.y += 3.5f * CONST;
	}
	if (coin.animTime <= 0) coin.active = false;
	if (coin.active) renderTexture(miscTexture, &coin.srcRect, &coin.destRect, 0, &entityCenter, SDL_FLIP_NONE);
	coin.animTime--;
}

void renderStar()
{
	if (star.active)
	{
		if (currentTime > star.lastFrameTime + 30 && !stopMovement)
		{
			star.lastFrameTime = currentTime;
			star.frameIndex = (star.frameIndex + 1) % 4;
		}
		star.srcRect = { star.frameIndex * 17.0f, 13 * 17, 16, 16 };
		renderTexture(miscTexture, &star.srcRect, &star.rect, 0, &entityCenter, SDL_FLIP_NONE);
	}
}

void rendergrowMushrooms()
{
	if (growMushroom.active)
	{
		renderTexture(growMushroomTexture, NULL, &growMushroom.rect, 0, &entityCenter, SDL_FLIP_NONE);
	}
}

void renderhealthMushrooms()
{
	if (healthMushroom.active)
	{
		renderTexture(healthMushroomTexture, NULL, &healthMushroom.rect, 0, &entityCenter, SDL_FLIP_NONE);
	}
}

void playerAnimation()
{
	player.flip = player.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
	player.animFrameDelay = (int) ((80 * CONST) / abs(player.velocity.x));
	player.sliding = ((player.velocity.x < 0 && player.oldvelocity_x < player.velocity.x && keys[SDL_SCANCODE_RIGHT]) || (player.velocity.x > 0 && player.oldvelocity_x > player.velocity.x && keys[SDL_SCANCODE_LEFT])) && !(keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT]);
	/*player.rect.h = player.isBig ? CELL_SIZE * 4.0f : CELL_SIZE * 2.0f;*/
	if (player.rect.y > camera.y + camera.h) player.isDied = true;
	if (player.isDied && player.life > 0)
	{
		if (currentTime > player.lastFrameTime + 3000) restart();
	}
	if (player.life <= 0) {
		running = false;
	}
	if (player.invisibleAnimation > 0)
	{
		player.invisibleAnimation--;
	}
	else if (player.invisibleAnimation <= 0)
	{
		if (player.starMode) player.starMode = false;
		player.canDie = true;
	}
	if (player.growAnimation > 0)
	{
		player.growAnimation--;
		stopMovement = true;
	}
	else if (player.growAnimation == 0)
	{
		stopMovement = false;
		player.growAnimation--;
	}
	if (!stopMovement && (player.invisibleAnimation / 10) % 2 == 0)
	{
		if (player.velocity.x != 0 && !player.jumping && !player.sliding && !player.isDied)
		{
			if (currentTime > player.lastFrameTime + player.animFrameDelay && player.grounded)
			{
				player.lastFrameTime = currentTime;
				player.frameIndex = (player.frameIndex + 1) % 3;
			}
			if (player.isBig)
			{
				renderTexture(playerBigRunFrames[player.frameIndex], NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				renderTexture(playerSmallRunFrames[player.frameIndex], NULL, &player.rect, 0, &entityCenter, player.flip);
			}
		}
		else if (player.flagAnimation && player.velocity.x == 0)
		{
			if (player.isBig)
			{
				renderTexture(playerBigFlag, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				renderTexture(playerSmallFlag, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
		}
		else if (player.sliding && !player.jumping && !player.isDied)
		{
			if (player.isBig)
			{
				renderTexture(playerBigSlideFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				renderTexture(playerSmallSlideFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
		}
		else if (player.jumping && !player.isDied)
		{
			if (player.isBig)
			{
				renderTexture(playerBigJumpFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				renderTexture(playerSmallJumpFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
		}
		else if (player.isDied)
		{
			renderTexture(playerSmallDeathFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
		}
		else
		{
			if (player.isBig)
			{
				renderTexture(playerBigIdleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				renderTexture(playerSmallIdleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
		}
	}
	else
	{
		if (!player.isBig)
		{
			if (player.growAnimation <= 45 && player.growAnimation >= 38 || player.growAnimation <= 33 &&
				player.growAnimation >= 30 || player.growAnimation <= 25 && player.growAnimation >= 22 &&
				player.growAnimation <= 13 && player.growAnimation >= 10 || player.growAnimation == 1)
			{
				player.rect.y = player.originalY;
				player.rect.h = CELL_SIZE * 2;
				renderTexture(playerSmallIdleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else if (player.growAnimation <= 37 && player.growAnimation >= 34 || player.growAnimation <= 29 &&
				player.growAnimation >= 26 || player.growAnimation <= 21 && player.growAnimation >= 18 ||
				player.growAnimation <= 9 && player.growAnimation >= 6)
			{
				player.rect.y = player.originalY - CELL_SIZE;
				player.rect.h = CELL_SIZE * 3;
				renderTexture(playerMiddleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				if (player.growAnimation > 0)
				{
					player.rect.y = player.originalY - CELL_SIZE * 2;
					player.rect.h = CELL_SIZE * 4;
					renderTexture(playerBigIdleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
				}
				else if (player.growAnimation == 0)
				{
					player.isBig = true;
					player.rect.y = player.originalY - CELL_SIZE * 2;
					player.rect.h = CELL_SIZE * 4;
				}
			}
		}
		else
		{
			if (player.growAnimation <= 45 && player.growAnimation >= 38 || player.growAnimation <= 33 &&
				player.growAnimation >= 30 || player.growAnimation <= 25 && player.growAnimation >= 22 &&
				player.growAnimation <= 13 && player.growAnimation >= 10 || player.growAnimation == 1)
			{
				player.rect.y = player.originalY;
				player.rect.h = CELL_SIZE * 4;
				renderTexture(playerBigIdleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else if (player.growAnimation <= 37 && player.growAnimation >= 34 || player.growAnimation <= 29 &&
				player.growAnimation >= 26 || player.growAnimation <= 21 && player.growAnimation >= 18 ||
				player.growAnimation <= 9 && player.growAnimation >= 6)
			{
				player.rect.y = player.originalY + CELL_SIZE;
				player.rect.h = CELL_SIZE * 3;
				renderTexture(playerMiddleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
			}
			else
			{
				if (player.growAnimation > 0)
				{
					player.rect.y = player.originalY + CELL_SIZE * 2;
					player.rect.h = CELL_SIZE * 2;
					renderTexture(playerSmallIdleFrame, NULL, &player.rect, 0, &entityCenter, player.flip);
				}
				else if (player.growAnimation == 0)
				{
					player.isBig = false;
					player.oldpos.y = player.originalY + CELL_SIZE * 2;
					player.rect.y = player.originalY + CELL_SIZE * 2;
					player.rect.h = CELL_SIZE * 2;
					player.canDie = false;
					player.invisibleAnimation = 200;
				}
			}
		}
	}
}

void veriticalMovement()
{
	if (!player.flagAnimation)
	{
		player.velocity.y += gravity;
	}
	else
	{
		player.velocity.y = 2 * CONST;
	}
	player.falling = player.velocity.y > gravity || !keys[SDL_SCANCODE_X];
	if (keys[SDL_SCANCODE_X] && !player.jumping && tmpKey == false && !player.isDied && !player.falling)
	{
		player.jumpTime = 2.45f * CONST;
		player.velocity.y = -player.jumpTime;
		player.jumping = true;
		player.grounded = false;
	}
	else if (keys[SDL_SCANCODE_X] && !player.falling && player.jumpTime < 3.45f * CONST && !player.isDied)
	{
		player.jumpTime += 0.1f * CONST;
		player.velocity.y = min(player.velocity.y, -player.jumpTime);
	}
	else if (player.falling && !player.isDied)
	{
		player.jumpTime = 3.45f * CONST;
		player.grounded = false;
	}
	player.rect.y += player.velocity.y;
	if (!player.isDied)
	{
		handleTileCollision(&player.rect, &player.oldpos);
	}
}

void horizontalMovement()
{
	maxSpeed = (keys[SDL_SCANCODE_Z] && player.grounded) ? 3 * CONST : 1.5f * CONST;
	acceleration = (keys[SDL_SCANCODE_Z] && player.grounded && player.velocity.x >= 2) ? 0.2f * CONST : 0.1f * CONST;
	// friction = (keys[SDL_SCANCODE_Z] && player.grounded) ? 0.04f * CONST : 0.04f * CONST;
	if (keys[SDL_SCANCODE_RIGHT] && player.velocity.x < maxSpeed)
	{
		player.velocity.x += acceleration;
		if (player.velocity.x > maxSpeed) player.velocity.x = maxSpeed;
	}
	if (keys[SDL_SCANCODE_LEFT] && player.velocity.x > -maxSpeed)
	{
		player.velocity.x -= acceleration;
		if (player.velocity.x < -maxSpeed) player.velocity.x = -maxSpeed;
	}
	if (player.velocity.x > 0 && !player.jumping)
	{
		player.facingRight = true;
		player.velocity.x -= friction;
		if (player.velocity.x < 0) player.velocity.x = 0;
	}
	else if (player.velocity.x < 0 && !player.jumping)
	{
		player.facingRight = false;
		player.velocity.x += friction;
		if (player.velocity.x > 0) player.velocity.x = 0;
	}
	if (player.rect.x < camera.x)
	{
		if (keys[SDL_SCANCODE_LEFT]) player.velocity.x = -1;
		else
		{
			player.velocity.x = 0;
		}
		if (keys[SDL_SCANCODE_RIGHT]) player.velocity.x = 0;
		player.rect.x = camera.x;
	}
	if (player.flagAnimation)
	{
		if (!player.grounded) player.velocity.x = 0;
		else
		{
			player.velocity.x = 1.5f;
		}
	}
	if (!player.isDied) player.rect.x += player.velocity.x;
}

void loadCollision(bool restart)
{
	ifstream file("assets/TileMap/collisions.map");
	if (!file) {
		cerr << "Failed to open file: " << "collisions.map" << std::endl;
		return;
	}

	for (int row = 0; row < HEIGHT_CELL / 2; row++) {
		for (int col = 0; col < WIDTH_CELL / 2; col++) {
			file >> collisiontiles[row][col].tilemap;
			collisiontiles[row][col].destRect = { (float)col * CELL_SIZE * 2 + 1 * CONST, (float)row * CELL_SIZE * 2 + 1 * CONST, CELL_SIZE * 2 - 2 * CONST, CELL_SIZE * 2 - 2 * CONST };
			collisiontiles[row][col].originalY = collisiontiles[row][col].destRect.y;
			if (collisiontiles[row][col].tilemap == 9 && restart) collisiontiles[row][col].tilemap = -1;
			collisiontiles[row][col].restart();
		}
	}
	
	file.close();
}

void handleEnemyEnemyCollision(Enemy* enemy, Enemy* otherEnemy, vector2* oldpos, vector2* oldOtherEnemyPos)
{
	if (enemy->rect.x + enemy->rect.w > otherEnemy->rect.x &&
		otherEnemy->rect.x + otherEnemy->rect.w > enemy->rect.x &&
		enemy->rect.y + enemy->rect.h > otherEnemy->rect.y &&
		otherEnemy->rect.y + otherEnemy->rect.h > enemy->rect.y)
	{
		if (oldpos->y + enemy->rect.h <= otherEnemy->rect.y && enemy->rect.y + enemy->rect.h >= otherEnemy->rect.y)
		{
			enemy->rect.y = otherEnemy->rect.y - enemy->rect.h;
			otherEnemy->rect.y = enemy->rect.y + enemy->rect.h;
		}
		else if (oldpos->x >= oldOtherEnemyPos->x + otherEnemy->rect.w && enemy->rect.x <= otherEnemy->rect.x + otherEnemy->rect.w)
		{
			enemy->rect.x = otherEnemy->rect.x + otherEnemy->rect.w;
			otherEnemy->rect.x = enemy->rect.x - enemy->rect.w;
			if (enemy->isDiedByPlayer && enemy->velocity.x > 0)
			{
				otherEnemy->isDiedByStar = true;
			}
			else if (!enemy->isDiedByPlayer || enemy->isDiedByPlayer && enemy->velocity.x == 0)
			{
				enemy->direction.x = -enemy->direction.x;
			}
		}
		else if (oldpos->x + enemy->rect.w <= oldOtherEnemyPos->x && enemy->rect.x + enemy->rect.w >= otherEnemy->rect.x)
		{
			enemy->rect.x = otherEnemy->rect.x - enemy->rect.w;
			otherEnemy->rect.x = enemy->rect.x + enemy->rect.w;
			if (enemy->isDiedByPlayer && enemy->velocity.x > 0)
			{
				otherEnemy->isDiedByStar = true;
			}
			else if (!enemy->isDiedByPlayer || enemy->isDiedByPlayer && enemy->velocity.x == 0)
			{
				enemy->direction.x = -enemy->direction.x;
			}
		}
		else if (oldpos->y >= oldOtherEnemyPos->y + otherEnemy->rect.h && enemy->rect.y <= otherEnemy->rect.y + otherEnemy->rect.h)
		{
			enemy->rect.y = otherEnemy->rect.y + otherEnemy->rect.h;
			otherEnemy->rect.y = enemy->rect.y - enemy->rect.h;
		}
	}
}

void handlePlayerEnemyCollision(Player* player, Enemy* enemy, vector2* oldplayerpos, vector2* oldenemypos)
{
	if (player->rect.x + player->rect.w > enemy->rect.x &&
		enemy->rect.x + enemy->rect.w > player->rect.x &&
		player->rect.y + player->rect.h > enemy->rect.y &&
		enemy->rect.y + enemy->rect.h > player->rect.y)
	{
		if (oldplayerpos->y + player->rect.h <= enemy->rect.y && player->rect.y + player->rect.h >= enemy->rect.y)
		{
			if (!player->starMode)
			{
				player->velocity.y = -2.4f * CONST;
				player->grounded = false;
				if (enemy->isDiedByPlayer)
				{
					if (oldplayerpos->x < oldenemypos->x)
					{
						enemy->direction.x = 1;
						enemy->velocity.x = 3;
					}
					else if (oldplayerpos->x > oldenemypos->x)
					{
						enemy->direction.x = -1;
						enemy->velocity.x = 3;
					}
				}
				enemy->isDiedByPlayer = true;
			}
			else
			{
				enemy->isDiedByStar = true;
			}
		}
		else if (oldplayerpos->x >= oldenemypos->x + enemy->rect.w && player->rect.x <= enemy->rect.x + enemy->rect.w)
		{
			if (!player->starMode)
			{
				if (!enemy->isDiedByPlayer && !enemy->isDiedByStar)
				{
					if (player->isBig)
					{
						player->originalY = player->rect.y;
						player->growAnimation = 45;
					}
					else
					{
						if (player->canDie)
						{
							player->velocity.x = 0;
							player->isDied = true;
							player->velocity.y = -4.8f * CONST;
						}
					}
				}
				else if (enemy->isDiedByPlayer)
				{
					if (enemy->velocity.x == 0)
					{
						enemy->direction.x = -1;
						enemy->velocity.x = 3;
					}
					else if (enemy->velocity.x != 0)
					{
						if (player->isBig)
						{
							player->originalY = player->rect.y;
							player->growAnimation = 45;
						}
						else
						{
							if (player->canDie)
							{
								player->velocity.x = 0;
								player->isDied = true;
								player->velocity.y = -4.8f * CONST;
							}
						}
					}
				}
			}
			else
			{
				enemy->isDiedByStar = true;
			}
		}
		else if (oldplayerpos->x + player->rect.w <= oldenemypos->x && player->rect.x + player->rect.w >= enemy->rect.x)
		{
			if (!player->starMode)
			{
				if (!enemy->isDiedByPlayer && !enemy->isDiedByStar)
				{
					if (player->isBig)
					{
						player->originalY = player->rect.y;
						player->growAnimation = 45;
					}
					else
					{
						if (player->canDie)
						{
							player->velocity.x = 0;
							player->isDied = true;
							player->velocity.y = -4.8f * CONST;
						}
					}
				}
				else if (enemy->isDiedByPlayer)
				{
					if (enemy->velocity.x == 0)
					{
						enemy->direction.x = 1;
						enemy->velocity.x = 3;
					}
					else if (enemy->velocity.x != 0)
					{
						if (player->isBig)
						{
							player->originalY = player->rect.y;
							player->growAnimation = 45;
						}
						else
						{
							if (player->canDie)
							{
								player->velocity.x = 0;
								player->isDied = true;
								player->velocity.y = -4.8f * CONST;
							}
						}
					}
				}
			}
			else
			{
				enemy->isDiedByStar = true;
			}
		}
		else if (oldplayerpos->y >= oldenemypos->y + enemy->rect.h && player->rect.y <= enemy->rect.y + enemy->rect.h)
		{
			if (!player->starMode)
			{
				if (player->isBig)
				{
					player->originalY = player->rect.y;
					player->growAnimation = 45;
				}
				else
				{
					if (player->canDie)
					{
						player->velocity.x = 0;
						player->isDied = true;
						player->velocity.y = -4.8f * CONST;
					}
				}
			}
			else
			{
				enemy->isDiedByStar = true;
			}
		}
	}
}

void handleTileCollision(SDL_FRect* entity, vector2* oldPos)
{
	for (int row = 0; row < HEIGHT_CELL / 2; row++) {
		for (int col = 0; col < WIDTH_CELL / 2; col++) {
			if (collisiontiles[row][col].tilemap == -1 || !collisiontiles[row][col].active) continue;
			if (entity->x + entity->w > collisiontiles[row][col].destRect.x &&
				collisiontiles[row][col].destRect.x + collisiontiles[row][col].destRect.w > entity->x &&
				entity->y + entity->h > collisiontiles[row][col].destRect.y &&
				collisiontiles[row][col].destRect.y + collisiontiles[row][col].destRect.h > entity->y)
				{
					if (entity == &player.rect && collisiontiles[row][col].tilemap == 12)
					{
						player.flagAnimation = true;
						if (player.velocity.x == 0) player.rect.x = collisiontiles[row][col].destRect.x + 8 - player.rect.w;
					}
					if (entity == &player.rect && collisiontiles[row][col].tilemap == 13) running = false;
					if (oldPos->y + entity->h <= collisiontiles[row][col].destRect.y &&
						collisiontiles[row][col].tilemap != 9 && collisiontiles[row][col].tilemap != 12)
					{
						entity->y = collisiontiles[row][col].destRect.y - entity->h;
						for (Enemy& firstEnemy : firstEnemies)
						{
							if (entity == &firstEnemy.rect)
							{
								firstEnemy.velocity.y = 0;
							}
						}
						for (Enemy& secondEnemy : secondEnemies)
						{
							if (entity == &secondEnemy.rect)
							{
								secondEnemy.velocity.y = 0;
							}
						}
						if (entity == &growMushroom.rect)
						{
							growMushroom.velocity.y = 0;
						}
						if (entity == &healthMushroom.rect)
						{
							healthMushroom.velocity.y = 0;
						}
						if (entity == &star.rect)
						{
							star.velocity.y = -5 * CONST;
						}
						if (entity == &player.rect)
						{
							player.velocity.y = 0;
							tmpKey = keys[SDL_SCANCODE_X];
							player.jumping = false;
							player.grounded = true;
						}
					}
					else if (oldPos->x >= collisiontiles[row][col].destRect.x + collisiontiles[row][col].destRect.w &&
						collisiontiles[row][col].tilemap != 9 && collisiontiles[row][col].tilemap != 12)
					{
						entity->x = collisiontiles[row][col].destRect.x + collisiontiles[row][col].destRect.w;
						for (Enemy& firstEnemy : firstEnemies)
						{
							if (entity == &firstEnemy.rect)
							{
								firstEnemy.direction.x = 1;
							}
						}
						for (Enemy& secondEnemy : secondEnemies)
						{
							if (entity == &secondEnemy.rect)
							{
								secondEnemy.direction.x = 1;
							}
						}
						if (entity == &growMushroom.rect)
						{
							growMushroom.direction.x = 1;
						}
						if (entity == &healthMushroom.rect)
						{
							healthMushroom.direction.x = 1;
						}
						if (entity == &star.rect)
						{
							star.direction.x = 1;
						}
						if (entity == &player.rect)
						{
							if (keys[SDL_SCANCODE_LEFT]) player.velocity.x = -1;
							else
							{
								player.velocity.x = 0;
							}
							if (keys[SDL_SCANCODE_RIGHT]) player.velocity.x = 0;
						}
					}
					else if (oldPos->x + entity->w <= collisiontiles[row][col].destRect.x &&
						collisiontiles[row][col].tilemap != 9 && collisiontiles[row][col].tilemap != 12)
					{
						entity->x = collisiontiles[row][col].destRect.x - entity->w;
						for (Enemy& firstEnemy : firstEnemies)
						{
							if (entity == &firstEnemy.rect)
							{
								firstEnemy.direction.x = -1;
							}

						}
						for (Enemy& secondEnemy : secondEnemies)
						{
							if (entity == &secondEnemy.rect)
							{
								secondEnemy.direction.x = -1;
							}
						}
						if (entity == &growMushroom.rect)
						{
							growMushroom.direction.x = -1;
						}
						if (entity == &healthMushroom.rect)
						{
							healthMushroom.direction.x = -1;
						}
						if (entity == &star.rect)
						{
							star.direction.x = -1;
						}
						if (entity == &player.rect)
						{	
							if (keys[SDL_SCANCODE_RIGHT]) player.velocity.x = 1;
							else
							{
								player.velocity.x = 0;
							}
							if (keys[SDL_SCANCODE_LEFT]) player.velocity.x = 0;
						}
					}
					else if (oldPos->y >= collisiontiles[row][col].destRect.y + collisiontiles[row][col].destRect.h && collisiontiles[row][col].tilemap != 12)
					{
						if (entity != &star.rect)
						{
							entity->y = collisiontiles[row][col].destRect.y + collisiontiles[row][col].destRect.h;
						}
						if (entity == &player.rect)
						{
							player.velocity.y = 0;
							if ((col + 1) < (WIDTH_CELL / 2) && (col - 1) >= 0)
							{
								if ((collisiontiles[row][col].tilemap == 3 || collisiontiles[row][col].tilemap == 4 || collisiontiles[row][col].tilemap == 9 ||
									collisiontiles[row][col].tilemap == 10 || collisiontiles[row][col].tilemap == 11) && !collisiontiles[row][col - 1].isCollided && !collisiontiles[row][col + 1].isCollided)
								{
									if (collisiontiles[row][col].tilemap == 11 && coin.number >= 0)
									{
										coin.active = true;
										coin.destRect = { collisiontiles[row][col].destRect.x ,collisiontiles[row][col].destRect.y - collisiontiles[row][col].destRect.h, collisiontiles[row][col].destRect.w, collisiontiles[row][col].destRect.h };;
										coin.animTime = 16;
										coin.frameIndex = 0;
									}
									if (collisiontiles[row][col].tilemap == 3)
									{
										coin.active = true;
										coin.destRect = { collisiontiles[row][col].destRect.x ,collisiontiles[row][col].destRect.y - collisiontiles[row][col].destRect.h, collisiontiles[row][col].destRect.w, collisiontiles[row][col].destRect.h };
										coin.animTime = 16;
										coin.frameIndex = 0;
										tile[2 * row][2 * col].tilemap = 31;
										tile[2 * row][2 * col + 1].tilemap = 32;
										tile[2 * row + 1][2 * col].tilemap = 35;
										tile[2 * row + 1][2 * col + 1].tilemap = 36;
										collisiontiles[row][col].tilemap = 0;
									}
									else if (collisiontiles[row][col].tilemap == 10)
									{
										star.active = true;
										star.rect = collisiontiles[row][col].destRect;
										star.animTimer = 17;
										star.frameIndex = 0;
										tile[2 * row][2 * col].tilemap = 31;
										tile[2 * row][2 * col + 1].tilemap = 32;
										tile[2 * row + 1][2 * col].tilemap = 35;
										tile[2 * row + 1][2 * col + 1].tilemap = 36;
										collisiontiles[row][col].tilemap = 0;
									}
									else if (collisiontiles[row][col].tilemap == 4)
									{
										growMushroom.active = true;
										growMushroom.rect = { collisiontiles[row][col].destRect.x, collisiontiles[row][col].originalY, collisiontiles[row][col].destRect.w, collisiontiles[row][col].destRect.h };
										growMushroom.animTimer = 17;
										tile[2 * row][2 * col].tilemap = 31;
										tile[2 * row][2 * col + 1].tilemap = 32;
										tile[2 * row + 1][2 * col].tilemap = 35;
										tile[2 * row + 1][2 * col + 1].tilemap = 36;
										collisiontiles[row][col].tilemap = 0;
									}
									else if (collisiontiles[row][col].tilemap == 9)
									{
										healthMushroom.active = true;
										healthMushroom.rect = { collisiontiles[row][col].destRect.x, collisiontiles[row][col].originalY, collisiontiles[row][col].destRect.w, collisiontiles[row][col].destRect.h };
										healthMushroom.animTimer = 17;
										tile[2 * row][2 * col].tilemap = 31;
										tile[2 * row][2 * col + 1].tilemap = 32;
										tile[2 * row + 1][2 * col].tilemap = 35;
										tile[2 * row + 1][2 * col + 1].tilemap = 36;
										collisiontiles[row][col].tilemap = 0;
									}
									collisiontiles[row][col].anim = true;
									collisiontiles[row][col].isCollided = true;
									collisiontiles[row][col].bounceTimer = 20;
								}
								else if ((collisiontiles[row][col].tilemap == 1 || collisiontiles[row][col].tilemap == 2) && !collisiontiles[row][col - 1].isCollided && !collisiontiles[row][col + 1].isCollided)
								{
									collisiontiles[row][col].anim = true;
									collisiontiles[row][col].isCollided = true;
									if (!player.isBig)
									{
										collisiontiles[row][col].bounceTimer = 20;
									}
									else
									{
										collisiontiles[row][col].bounceTimer = 80;
										angle = 0;
									}	
								}
							}
						}
					}
				}
			}
		}
	}

void updateBrick()
{
	for (int row = 0; row < HEIGHT_CELL / 2; row++) {
		for (int col = 0; col < WIDTH_CELL / 2; col++) {
			if (collisiontiles[row][col].anim) {
				for (Enemy& firstEnemy : firstEnemies)
				{
					if (SDL_HasRectIntersectionFloat(&collisiontiles[row][col].destRect, &firstEnemy.rect) && collisiontiles[row][col].active &&
						((collisiontiles[row][col].bounceTimer == 20 && !player.isBig) || (collisiontiles[row][col].bounceTimer == 80 && player.isBig)))
					{
						firstEnemy.isDiedByStar = true;
					}
				}
				for (Enemy& secondEnemy : secondEnemies)
				{
					if (SDL_HasRectIntersectionFloat(&collisiontiles[row][col].destRect, &secondEnemy.rect) && collisiontiles[row][col].active &&
						((collisiontiles[row][col].bounceTimer == 20 && !player.isBig) || (collisiontiles[row][col].bounceTimer == 80 && player.isBig)))
					{
						secondEnemy.isDiedByStar = true;
					}
				}
				if (SDL_HasRectIntersectionFloat(&collisiontiles[row][col].destRect, &growMushroom.rect) && collisiontiles[row][col].active &&
					growMushroom.rect.x != collisiontiles[row][col].destRect.x && growMushroom.rect.y != collisiontiles[row][col].destRect.y && collisiontiles[row][col].bounceTimer == 20)
				{
					growMushroom.velocity.y = -2.4f * CONST;
					growMushroom.direction.x = -growMushroom.direction.x;
				}
				if (collisiontiles[row][col].tilemap == 11 && collisiontiles[row][col].bounceTimer == 19) coin.number--;
				if (collisiontiles[row][col].tilemap == 11 && coin.number == 0 && collisiontiles[row][col].bounceTimer == 20)
				{
					tile[2 * row][2 * col].tilemap = 31;
					tile[2 * row][2 * col + 1].tilemap = 32;
					tile[2 * row + 1][2 * col].tilemap = 35;
					tile[2 * row + 1][2 * col + 1].tilemap = 36;
					collisiontiles[row][col].tilemap = 0;
				}
				if (collisiontiles[row][col].bounceTimer == 15) collisiontiles[row][col].isCollided = false;
				if (collisiontiles[row][col].bounceTimer > 10) {
					tile[2 * row][2 * col].destRect.y -= 0.75f * CONST;
					tile[2 * row][2 * col + 1].destRect.y -= 0.75f * CONST;
					tile[2 * row + 1][2 * col].destRect.y -= 0.75f * CONST;
					tile[2 * row + 1][2 * col + 1].destRect.y -= 0.75f * CONST;
					/*collisiontiles[row][col].destrect.y -= 0.75f * const;*/
				}
				else {
					tile[2 * row][2 * col].destRect.y += 0.75f * CONST;
					tile[2 * row][2 * col + 1].destRect.y += 0.75f * CONST;
					tile[2 * row + 1][2 * col].destRect.y += 0.75f * CONST;
					tile[2 * row + 1][2 * col + 1].destRect.y += 0.75f * CONST;
					/*collisiontiles[row][col].destRect.y += 0.75f * CONST;*/
				}

				if (player.isBig && (collisiontiles[row][col].tilemap == 1 || collisiontiles[row][col].tilemap == 2))
				{
					tile[2 * row][2 * col].active = false;
					tile[2 * row][2 * col + 1].active = false;
					tile[2 * row + 1][2 * col].active = false;
					tile[2 * row + 1][2 * col + 1].active = false;
					if (collisiontiles[row][col].bounceTimer == 79)
					{
						collisiontiles[row][col].active = false;
					}
					if (collisiontiles[row][col].bounceTimer == 75) collisiontiles[row][col].isCollided = false;
					else if (collisiontiles[row][col].bounceTimer > 60)
					{
						tile[2 * row][2 * col].destRect.y -= 3 * CONST;
						tile[2 * row][2 * col + 1].destRect.y -= 3 * CONST;
						tile[2 * row + 1][2 * col].destRect.y -= 0.5f * CONST;
						tile[2 * row + 1][2 * col + 1].destRect.y -= 0.5f * CONST;
					}
					else
					{
						tile[2 * row][2 * col].destRect.y += 5 * CONST;
						tile[2 * row][2 * col + 1].destRect.y += 5 * CONST;
						tile[2 * row + 1][2 * col].destRect.y += 5 * CONST;
						tile[2 * row + 1][2 * col + 1].destRect.y += 5 * CONST;
					}
					tile[2 * row][2 * col].destRect.x -= 1 * CONST;
					tile[2 * row][2 * col + 1].destRect.x += 1 * CONST;
					tile[2 * row + 1][2 * col].destRect.x -= 1 * CONST;
					tile[2 * row + 1][2 * col + 1].destRect.x += 1 * CONST;
					angle += 30;
					renderTexture(miscTexture, &srect1, &tile[2 * row][2 * col].destRect, -angle, &center, SDL_FLIP_HORIZONTAL);
					renderTexture(miscTexture, &srect2, &tile[2 * row][2 * col + 1].destRect, angle, &center, SDL_FLIP_HORIZONTAL);
					renderTexture(miscTexture, &srect3, &tile[2 * row + 1][2 * col].destRect, -angle, &center, SDL_FLIP_HORIZONTAL);
					renderTexture(miscTexture, &srect4, &tile[2 * row + 1][2 * col + 1].destRect, angle, &center, SDL_FLIP_HORIZONTAL);
				}

				collisiontiles[row][col].bounceTimer--;

				if (collisiontiles[row][col].bounceTimer <= 0) {
					/*collisiontiles[row][col].destRect.y = collisiontiles[row][col].originalY;*/
					tile[2 * row][2 * col].destRect.y = tile[2 * row][2 * col].originalY;
					tile[2 * row][2 * col + 1].destRect.y = tile[2 * row][2 * col + 1].originalY;
					tile[2 * row + 1][2 * col].destRect.y = tile[2 * row + 1][2 * col].originalY;
					tile[2 * row + 1][2 * col + 1].destRect.y = tile[2 * row + 1][2 * col + 1].originalY;
					collisiontiles[row][col].anim = false;
				}
			}
		}
	}
}

void renderTilemap()
{
	if (growMushroom.animTimer > 0) rendergrowMushrooms();
	if (healthMushroom.animTimer > 0) renderhealthMushrooms();
	if (star.animTimer > 0) renderStar();
	for (int row = 0; row < HEIGHT_CELL; row++) {
		for (int col = 0; col < WIDTH_CELL; col++) {
			if (tile[row][col].tilemap == 0) continue;
			SDL_FRect srcRect = { (float)(tile[row][col].tilemap % 32) * 8, (float)floor(tile[row][col].tilemap / 32) * 8, 8, 8 };
			if (tile[row][col].destRect.x <= camera.x + camera.w && tile[row][col].destRect.x + tile[row][col].destRect.w >= camera.x && tile[row][col].active)
			{
				renderTexture(tileset_texture, &srcRect, &tile[row][col].destRect, 0, &center, SDL_FLIP_NONE);
			}
		}
	}
	if (growMushroom.animTimer <= 0) rendergrowMushrooms();
	if (healthMushroom.animTimer <= 0) renderhealthMushrooms();
	if (star.animTimer <= 0) renderStar();
}

void loadTilemap() {
	ifstream file("assets/TileMap/tiledAll.map");
	if (!file) {
		cerr << "Failed to open file: " << "tiledAll.map" << std::endl;
		return; 
	}
	for (int row = 0; row < HEIGHT_CELL; row++) {
		for (int col = 0; col < WIDTH_CELL; col++) {
			file >> tile[row][col].tilemap;
			tile[row][col].destRect = { (float)col * CELL_SIZE, (float)row * CELL_SIZE, CELL_SIZE, CELL_SIZE };
			tile[row][col].originalY = tile[row][col].destRect.y;
			tile[row][col].restart();
		}
	}

	file.close();
}

void restart()
{
	player.life--;
	player.rect = { CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE , CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[0].rect = { 44 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[1].rect = { 80 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[2].rect = { 102 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[3].rect = { 105 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[4].rect = { 160 * CELL_SIZE, WINDOW_HEIGHT - 22 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[5].rect = { 164 * CELL_SIZE, WINDOW_HEIGHT - 22 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[6].rect = { 194 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[7].rect = { 197 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	secondEnemies[0].rect = { 214 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[8].rect = { 228 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[9].rect = { 231 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[10].rect = { 248 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[11].rect = { 251 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[12].rect = { 256 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[13].rect = { 259 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[14].rect = { 348 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[15].rect = { 351 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	camera = { 0 , 0 , WINDOW_WIDTH, WINDOW_HEIGHT };
	for (Enemy& firstEnemy : firstEnemies) firstEnemy.restart();
	for (Enemy& secondEnemy : secondEnemies) secondEnemy.restart();
	healthMushroom.restart();
	growMushroom.restart();
	coin.restart();
	loadTilemap();
	loadCollision(true);
	player.restart();	
}
void initialize()
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		cerr << "SDL Init Error: " << SDL_GetError() << endl;
		running = false;
	}

	if (!SDL_CreateWindowAndRenderer("Super Player Bros", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
	{
		cerr << "SDL Init Error: " << SDL_GetError() << endl;
		running = false;
	}

	player.rect = { CELL_SIZE * 5, WINDOW_HEIGHT - 6 * CELL_SIZE , CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[0].rect = { 44 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[1].rect = { 80 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[2].rect = { 102 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[3].rect = { 105 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[4].rect = { 160 * CELL_SIZE, WINDOW_HEIGHT - 22 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[5].rect = { 164 * CELL_SIZE, WINDOW_HEIGHT - 22 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[6].rect = { 194 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[7].rect = { 197 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	secondEnemies[0].rect = { 214 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2};
	firstEnemies[8].rect = { 228 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[9].rect = { 231 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[10].rect = { 248 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[11].rect = { 251 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[12].rect = { 256 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[13].rect = { 259 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[14].rect = { 348 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	firstEnemies[15].rect = { 351 * CELL_SIZE, WINDOW_HEIGHT - 6 * CELL_SIZE, CELL_SIZE * 2, CELL_SIZE * 2 };
	camera = { 0 , 0 , WINDOW_WIDTH, WINDOW_HEIGHT };
	tileset_texture = IMG_LoadTexture(renderer, "assets/TileMap/tiles.png");
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
	firstEnemyWalkFrames[0] = IMG_LoadTexture(renderer, "assets/Enemy_1/Enemy_1_Walk1.png");
	firstEnemyWalkFrames[1] = IMG_LoadTexture(renderer, "assets/Enemy_1/Enemy_1_Walk2.png");
	firstEnemyPlayerKilledFrame = IMG_LoadTexture(renderer, "assets/Enemy_1/Enemy_1_Flat.png");
	firstEnemyDeathFrame = IMG_LoadTexture(renderer, "assets/Enemy_1/Enemy_1_Death.png");
	secondEnemyWalkFrames[0] = IMG_LoadTexture(renderer, "assets/Enemy_2/Enemy_2_Walk1.png");
	secondEnemyWalkFrames[1] = IMG_LoadTexture(renderer, "assets/Enemy_2/Enemy_2_Walk2.png");
	secondEnemyPlayerKilledFrame = IMG_LoadTexture(renderer, "assets/Enemy_2/Enemy_2_Shell.png");
	secondEnemyDeathFrame = IMG_LoadTexture(renderer, "assets/Enemy_2/Enemy_2_Death.png");
	growMushroomTexture = IMG_LoadTexture(renderer, "assets/Items/growMushroom.png");
	healthMushroomTexture = IMG_LoadTexture(renderer, "assets/Items/healthMushroom.png");
	miscTexture = IMG_LoadTexture(renderer, "assets/Misc/misc.png");
	if (!tileset_texture || !playerSmallIdleFrame || !playerSmallRunFrames || !playerSmallJumpFrame || 
		!firstEnemyWalkFrames || !firstEnemyDeathFrame || !firstEnemyPlayerKilledFrame ||
		!secondEnemyWalkFrames || !secondEnemyDeathFrame || !secondEnemyPlayerKilledFrame || !growMushroomTexture || !miscTexture)
	{
		cerr << "SDL Init Error: " << SDL_GetError() << endl;
		running = false;
	}
	loadTilemap();
	loadCollision(false);
}

void destroyEverything()
{
	SDL_DestroyTexture(tileset_texture);
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
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(firstEnemyWalkFrames[i]);
	}
	SDL_DestroyTexture(firstEnemyPlayerKilledFrame);
	SDL_DestroyTexture(firstEnemyDeathFrame);
	for (int i = 0; i < 2; i++) {
		SDL_DestroyTexture(secondEnemyWalkFrames[i]);
	}
	SDL_DestroyTexture(secondEnemyPlayerKilledFrame);
	SDL_DestroyTexture(secondEnemyDeathFrame);
	SDL_DestroyTexture(growMushroomTexture);
	SDL_DestroyTexture(miscTexture);
	SDL_DestroyTexture(playerMiddleFrame);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
		{
			running = false;
			break;
		}
		case SDL_EVENT_KEY_DOWN:
			keys[event.key.scancode] = true;
			break;
		case SDL_EVENT_KEY_UP:
			keys[event.key.scancode] = false;
			break;
		}
	}
}