#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#define WIDTH 960
#define HEIGHT 480
#define X_STEP 32
#define Y_STEP 32

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