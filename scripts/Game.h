#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <SDL3_image/SDL_image.h>
#include "Entity.h"
#include "Player.h"
#include "Insect.h"
#include "Turtle.h"
#include "InteractableType.h"
#include "GrowMushroom.h"
#include "HealthMushroom.h"
#include "Star.h"
#include "Coin.h"
#include "Brick.h"
#include "BrickPiece.h"
#include "QuestionBlock.h"
#include "Flag.h"
#include "FlagPole.h"
#include "FinishDoor.h"
#include "Camera.h"
#include "TileMap.h"
#include "CollisionManager.h"

struct EntityInfo
{
    float xPos;
    float yPos;
};

struct QuestionBlockInfo
{
    float xPos;
    float yPos;
    ItemType item;
    QuestionBlockState initialState;
    int itemCount;
};

class Game
{
public:
    void init();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void clean();
    void quit();
    bool running();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::array<bool, SDL_SCANCODE_COUNT> keys = { false };
    bool isRunning;
    std::vector<std::unique_ptr<Entity>> entities;
    Camera* camera;
    TileMap* tileMap;
    CollisionManager* collisionManager;
    EntityInfo insectInfos[insectCount];
    EntityInfo turtleInfo;
    EntityInfo brickInfos[brickCount];
    QuestionBlockInfo questionBlockInfos[questionBlockCount];
    int entityCount = 0;
    float commonAnimTimer = 0;
    float commonLastAnimTimer = 0;
    int commonFrameIndex = 0;
    const int commonFrameCount = 4;
    const float commonFirstFrameDelay = 0.39f;
    const float commonFrameDelay = 0.13f;
    std::unique_ptr<Player> player;
    std::unique_ptr<Insect> insects[insectCount];
    std::unique_ptr<Turtle> turtle;
    std::unique_ptr<GrowMushroom> growMushrooms[growMushroomCount];
    std::unique_ptr<HealthMushroom> healthMushrooms[healthMushroomCount];
    std::unique_ptr<Star> stars[starCount];
    std::unique_ptr<Coin> coins[coinCount];
    std::unique_ptr<Brick> bricks[brickCount];
    std::unique_ptr<BrickPiece> brickPieces[brickPieceCount];
    std::unique_ptr<QuestionBlock> questionBlocks[questionBlockCount];
    std::unique_ptr<FlagPole> flagPole;
    std::unique_ptr<Flag> flag;
    std::unique_ptr<FinishDoor> finishDoor;
    std::vector<BrickPiece*> brickPiecesOriginals;
    std::vector<InteractableType*> interactableTypes;
};