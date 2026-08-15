#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <SDL3_image/SDL_image.h>
#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"
#include "CollisionManager.h"

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
    bool keys[SDL_SCANCODE_COUNT] = { false };
    bool isRunning;
    std::vector<std::unique_ptr<Entity>> entities;
    Camera* camera;
    TileMap* tileMap;
    std::unique_ptr<Player> player;
    CollisionManager* collisionManager;
};