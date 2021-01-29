#pragma once
#include <vector>
#include <string>

#include "GameGlobal.h"
#include "GameDefine.h"
#include "PairTile.h"

class MapTile
{
protected:
	unsigned x, y;
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);	
	short int level;
	TileType type;
	int cost;
public:
	MapTile()
	{
		this->x = 0;
		this->y = 0;
		position.x = x;
		position.y = y;
	}

	MapTile(int x, int y, int level = 3)
	{
		this->x = x;
		this->y = y;
		position.x = x;
		position.y = y;
		this->level = level;
		type = Empty;
	}

	MapTile(const MapTile &tile)
	{ 
		x = tile.x; y = tile.y;
		position = tile.position;
		level = tile.level;
		type = tile.type;
	}

	unsigned GetX() { return x; }
	unsigned GetY() { return y; }

	void SetType(TileType type) { this->type = type; }

	TileType GetType() { return type; }

	void SetCost(TileType type = Empty, int _cost = 0)
	{
		switch (level)
		{
		case 1:			
		case 2:
			cost = _cost;
			break;
		case 3:
			if (type == Border || type == Metal || type == WaterBrick) cost = -1;
			if (type == NormalBrick || type == BrickGate) cost = 2;
			if (type == Empty || type == Gate) cost = 1;
			break;
		default:
			break;
		}		
	}

	int GetCost() { return cost; }
	
	bool operator== (const MapTile& t) const { return (x == t.x && y == t.y); }

	MapTile& operator= (const MapTile &tile)
	{
		x = tile.x; y = tile.y;
		position = tile.position;
		level = tile.level;
		type = tile.type;

		return *this;
	}
};

class MapTileLv3 : public MapTile
{
public:
	MapTileLv3() : MapTile() { level = 3; }
	MapTileLv3(int x, int y, int level = 3) : MapTile(x, y, level) {}

	using MapTile::operator=;
};

class MapTileLv2 : public MapTile
{
public:
	std::array<std::array<MapTileLv3*, LEVEL3_HEIGHT>, LEVEL3_WIDTH> childTile;	

	MapTileLv2() : MapTile() { level = 2; }
	MapTileLv2(int x, int y, int level = 2) : MapTile(x, y, level) { cost = 1; }
	void AddChildTile(MapTileLv3* tile)
	{
		if (childTile[tile->GetX()][tile->GetY()] == nullptr)
			childTile[tile->GetX()][tile->GetY()] = new MapTileLv3();
		childTile[tile->GetX()][tile->GetY()] = tile;
	}
};

class MapTileLv1 : public MapTile
{
public:
	std::array<std::array<MapTileLv2*, LEVEL2_HEIGHT / LEVEL1_HEIGHT>, LEVEL2_WIDTH / LEVEL1_WIDTH> childTile;
	MapTileLv1() : MapTile() { level = 1; }
	MapTileLv1(int x, int y, int level = 1) : MapTile(x, y, level) { cost = 1; }
	void AddChildTile(MapTileLv2* tile)
	{
		if (childTile[tile->GetX()][tile->GetY()] == nullptr)
			childTile[tile->GetX()][tile->GetY()] = new MapTileLv2();
		childTile[tile->GetX()][tile->GetY()] = tile;
	}
};