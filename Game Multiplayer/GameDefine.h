#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#define WIDTH 960
#define HEIGHT 480
#define MAP_WIDTH 3840
#define MAP_HEIGHT 3840
#define X_TILE_COUNT 120
#define Y_TILE_COUNT 120
#define X_STEP 32
#define Y_STEP 32

#define LEVEL1_WIDTH	2
#define LEVEL1_HEIGHT	2
#define LEVEL2_WIDTH	8
#define LEVEL2_HEIGHT	8
#define LEVEL3_WIDTH	15
#define LEVEL3_HEIGHT	15

#define PART_SIZE	12

#define ASTAR_VALUE_PLAYER	1
#define ASTAR_VALUE_NPC		1
#define ASTAR_VALUE_BRICK	1
#define ASTAR_VALUE_EAGLE	1
#define ASTAR_VALUE_ITEM	0

#define SHIELD_TIME_MAX		7
#define SPAWN_ITEM_DELAY	1

const bool IS_DRAW_ASTAR_PATH = true;

const int MAX_X_TILE = 120;
const int MAX_Y_TILE = 120;

const int SIZE_MAP_X = MAX_X_TILE * X_STEP;
const int SIZE_MAP_Y = MAX_Y_TILE * Y_STEP;

const int NUM_PART_X = MAX_X_TILE / PART_SIZE;
const int NUM_PART_Y = MAX_X_TILE / PART_SIZE;

enum EntityType
{
	ET_Unknown,
	ET_Player, 
	ET_Bullet,
	ET_Boundary, 
	ET_NormalBrick, 
	ET_MetalBrick, 
	ET_Water,
	ET_ProtectItem, 
	ET_HealItem,
	ET_UpgradeItem,
	ET_NPC,
	ET_EaglePlayer,
	ET_EagleNPC
};

// [0, 4] => 3 bit
enum Direction
{
	D_Stand,
	D_Left,
	D_Right,
	D_Up,
	D_Down
};

enum TileType
{
	Empty,
	Opened,
	Closed,
	Obstacle,
	Moving,
	Begin,
	Destination,
	Path,
	Path_Dodging,
	Path_Attacking
};

struct Vector2D
{
	int x;
	int y;

	Vector2D() : x(0), y(0)
	{
	}

	Vector2D(int x1, int y1) : x(x1), y(y1)
	{
	}

	void set(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	int distance(const Vector2D &other) const
	{
		return abs(other.x - x) + abs(other.y - y);
	}

	bool operator== (const Vector2D &other) const
	{
		return x == other.x && y == other.y;
	}
};