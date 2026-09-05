#pragma once

#define TILE_SIZE 8
#define CELL_SIZE 16
#define WINDOW_WIDTH_TILE 32
#define CAMERA_WIDTH_TILE 40
#define MAP_WIDTH_TILE 424
#define MAP_HEIGHT_TILE 30
#define CAMERA_WIDTH (CAMERA_WIDTH_TILE * CELL_SIZE)
#define WINDOW_WIDTH (WINDOW_WIDTH_TILE * CELL_SIZE)
#define WINDOW_HEIGHT (MAP_HEIGHT_TILE * CELL_SIZE)

enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

enum class EntityType
{
	PlayerType,
	EnemyType,
	InteractableType
};