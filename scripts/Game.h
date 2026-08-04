#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <SDL3_image/SDL_image.h>

#define CELL_SIZE 16
#define WIDTH_CELL 424
#define HEIGHT_CELL 30
#define WINDOW_WIDTH (32 * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT_CELL * CELL_SIZE)
#define CONST (CELL_SIZE / 8)

class Entity;

class Game
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning = true;
    std::vector<std::unique_ptr<Entity>> entities;
public:
    void init();
    void handleEvents();
    void update();
    void render();
    void clean();
    void quit();
    bool running();
};