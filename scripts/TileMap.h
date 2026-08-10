#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <fstream>
#include "Constants.h"
#include "Camera.h"

struct Tile
{
	int tileID;
	SDL_FRect rect;
	float originalY;
	float animTimer = 0;
	float lastAnimTimer = 0;
};

class TileMap
{
public:
	void init(SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer, Camera* camera);
private:
	Tile tiles[MAP_HEIGHT_TILE][MAP_WIDTH_TILE];
	SDL_FPoint center = { TILE_SIZE, TILE_SIZE };
	SDL_Texture* tileSetTexture;
	void loadTileMap();
};