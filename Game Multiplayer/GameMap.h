#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include <array>
#include <set>

#include "Sprite.h"	
#include "MapReader\Tmx.h.in"
#include "GameGlobal.h"
#include "Brick.h"
#include "BrickNormal.h"
#include "MetalBrick.h"
#include "Water.h"
#include "Boundary.h"
#include "GameCollision.h"
#include "GameDebugDraw.h"
#include "QuadTree.h"
#include "Camera.h"
#include "MapTile.h"

class GameMap
{
	const int MAX_LEVEL2_TILE = 8;	

	std::vector<std::vector<Water*>> _waterList;
	std::vector<std::vector<Brick*>> _brickList;
	std::vector<std::vector<BrickNormal*>> _brickNorList;

	std::array<std::array<MapTileLv1*, LEVEL1_HEIGHT>, LEVEL1_WIDTH> gridMapLv1;
	std::array<std::array<MapTileLv2*, LEVEL2_HEIGHT>, LEVEL2_WIDTH> gridMapLv2;
	std::array<std::array<MapTileLv3*, LEVEL3_HEIGHT * LEVEL2_HEIGHT>,
										LEVEL3_WIDTH * LEVEL2_WIDTH> gridMapLv3;

	Tmx::Map *_map;
	std::map<int, Sprite*>  _tilesetList;
	GameDebugDraw* _debugDraw;

	void LoadMap(char* filePath);
	int getWidth() { return _map->GetWidth() * _map->GetTileWidth(); }
	int getHeight() { return _map->GetHeight() * _map->GetTileHeight(); }
	int getTileWidth() { return _map->GetTileWidth(); }
	int getTileHeight() { return _map->GetTileHeight(); }

public:
	std::set<int, std::greater <int>> obstaclesNodes;
	std::set<int, std::greater <int>> normalBrickNodes;

	GameMap(char* filePath);
	~GameMap() {}	

	Tmx::Map* getMap() { return _map; }
	std::vector<Brick*> getBrickList(int index) { return _brickList[index]; }
	std::vector<BrickNormal*> getBrickNorList(int index) { return _brickNorList[index]; }

	MapTileLv1* getMapTileLv1(int x, int y) { return gridMapLv1[x][y]; }
	MapTileLv2* getMapTileLv2(int x, int y) { return gridMapLv2[x][y]; }
	MapTileLv3* getMapTileLv3(int x, int y) { return gridMapLv3[x][y]; }

	bool IsValidTile(int x, int y) { return gridMapLv3[x][y]->GetCost() == -1; }

	void Draw();
	void Draw(Camera _camera);	
};

