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
#include "GrowMushroom.h"
#include "HealthMushroom.h"
#include "Star.h"
#include "Camera.h"
#include "TileMap.h"
#include "CollisionManager.h"

struct entityInfo
{
    float xPos;
    float yPos;
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
    entityInfo insectInfos[16];
    entityInfo turtleInfo;
    entityInfo growMushroomInfo;
    entityInfo healthMushroomInfo;
    entityInfo starInfo;
    int entityCount = 0;
    std::unique_ptr<Player> player;
    std::unique_ptr<Insect> insects[16];
    std::unique_ptr<Turtle> turtle;
    std::unique_ptr<GrowMushroom> growMushroom;
    std::unique_ptr<HealthMushroom> healthMushroom;
    std::unique_ptr<Star> star;
    CollisionManager* collisionManager;
};