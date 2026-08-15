#include "TileMap.h"
#include "Camera.h"

void TileMap::init(SDL_Renderer* renderer)
{
	tileSetTexture = IMG_LoadTexture(renderer, "assets/TileMap/tiles.png");
	SDL_SetTextureScaleMode(tileSetTexture, SDL_SCALEMODE_NEAREST);
	loadTileMap();
}

void TileMap::render(SDL_Renderer* renderer, Camera* camera)
{
	int firstCol = (int) camera->getRect().x / CELL_SIZE;
	int lastCol = firstCol + WINDOW_WIDTH_TILE + 2;
	for (int row = 0; row < MAP_HEIGHT_TILE; row++) {
		for (int col = firstCol; col < lastCol; col++) {
			if (tiles[row][col].tileID == 0) continue;
			SDL_FRect srcRect = { (float) (tiles[row][col].tileID % 32) * TILE_SIZE, (float) (tiles[row][col].tileID / 32) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_FRect newRect = {
				roundf(tiles[row][col].rect.x - camera->getRect().x),
				tiles[row][col].rect.y,
				tiles[row][col].rect.w,
				tiles[row][col].rect.h
			};
			SDL_RenderTextureRotated(renderer, tileSetTexture, &srcRect, &newRect, 0, &center, SDL_FLIP_NONE);
		}
	}
}

void TileMap::loadTileMap()
{
	std::ifstream file("assets/TileMap/tiledAll.map");
	if (!file) {
		std::cerr << "Failed to open file: " << "tiledAll.map" << std::endl;
		return;
	}
	for (int row = 0; row < MAP_HEIGHT_TILE; row++) {
		for (int col = 0; col < MAP_WIDTH_TILE; col++) {
			file >> tiles[row][col].tileID;
			tiles[row][col].rect = { (float)col * CELL_SIZE, (float) row * CELL_SIZE, CELL_SIZE, CELL_SIZE };
			tiles[row][col].originalY = tiles[row][col].rect.y;
		}
	}

	file.close();
}