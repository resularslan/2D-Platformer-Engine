#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <SDL3_image/SDL_image.h>
#include "Entity.h"

class Game
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool keys[SDL_SCANCODE_COUNT] = { false };
    bool isRunning;
    std::vector<std::unique_ptr<Entity>> entities;
public:
    void init();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void clean();
    void quit();
    bool running();
};