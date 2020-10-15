﻿#pragma once
#include "Entity.h"

enum CollisionSide
{
	CS_Left,
	CS_Right,
	CS_Top,
	CS_Bottom,
	CS_Unknown
};

struct CollisionResult
{
	bool IsCollided;
	RECT Rect;
	CollisionSide Side = CS_Unknown;
};

class GameCollision
{
public:
	// tính toán frame tiếp theo có va chạm hay không
	static bool	IsCollideInNextFrame(Entity* entity1, Entity* entity2, float dt);

	// xét cho enity1
	static CollisionResult getCollisionResult(Entity* entity1, Entity* entity2);
};

