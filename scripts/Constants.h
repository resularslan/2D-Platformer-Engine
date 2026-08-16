#pragma once

#define TILE_SIZE 8
#define COLLISION_TILE_SIZE 16
#define CELL_SIZE 16
#define WINDOW_WIDTH_TILE 32
#define MAP_WIDTH_TILE 424
#define MAP_HEIGHT_TILE 30
#define WINDOW_WIDTH (WINDOW_WIDTH_TILE * CELL_SIZE)
#define WINDOW_HEIGHT (MAP_HEIGHT_TILE * CELL_SIZE)

const float gravity = 0.2f;
const float friction = 4;

enum Direction
{
	Up,
	Down,
	Left,
	Right
};

enum EntityType
{
	PlayerType,
	EnemyType,
	GrowMushroomType,
	HealthMushroomType,
	StarType
};