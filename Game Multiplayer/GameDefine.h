#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#define WIDTH 960
#define HEIGHT 480
#define MAP_WIDTH 3840
#define MAP_HEIGHT 3840
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

// [0, 10] => 4 bit
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
	ET_UpgradeItem,
	ET_NPC,
	ET_Eagle
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
	NormalBrick,
	Metal,
	WaterBrick,
	Tree,
	SpawnPoint,
	Base,
	Gate,
	BrickGate,
	Border,

	Level2,
	Level1,

	Empty,
	Opened,
	Closed,
	Obstacle,
	Moving,
	Begin,
	Destination,
	Path,
	Debug0,
	Debug1
};

struct Vector2D
{
	int x; int y;
	Vector2D() : x(0), y(0) {}
	Vector2D(int _x, int _y) : x(_x), y(_y) {};

	void set(int _x, int _y) { x = _x; y = _y; }

	int distance(const Vector2D &v) const { return abs(v.x - x) + abs(v.y - y); }

	bool operator== (const Vector2D &v) const { return x == v.x && y == v.y; }
};