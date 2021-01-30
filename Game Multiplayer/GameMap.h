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
#include "Eagle.h"
#include "GameCollision.h"
#include "GameDebugDraw.h"
#include "QuadTree.h"
#include "Camera.h"
#include "MapTile.h"

class GameMap
{	
	std::array < std::array<Brick*, MAP_HEIGHT / Y_STEP>, MAP_WIDTH / X_STEP> _bricks;
	//std::array < std::array<Grass*, MAP_HEIGHT / Y_STEP>>, MAP_WIDTH / X_STEP > _grassies;

	std::vector<Brick*> _brickList;
	std::vector<BrickNormal*> _brickNorList;
	std::vector<Eagle*> _eagleList;
	//std::vector<Grass*> _grassList;

	Tmx::Map *_map;
	std::map<int, Sprite*>  _tilesetList;
	GameDebugDraw* _debugDraw;

	void LoadMap(char* filePath);
	int getWidth() { return _map->GetWidth() * _map->GetTileWidth(); }
	int getHeight() { return _map->GetHeight() * _map->GetTileHeight(); }
	int getTileWidth() { return _map->GetTileWidth(); }
	int getTileHeight() { return _map->GetTileHeight(); }

public:

	GameMap(char* filePath);
	~GameMap() {}	

	Tmx::Map* getMap() { return _map; }
	std::vector<Brick*> getBrickList() { return _brickList; }
	std::vector<BrickNormal*> getBrickNorList() { return _brickNorList; }
	std::vector<Eagle*> getEagleList() { return _eagleList; }

	std::vector<Brick*> getBrickListAroundEntity(int posX, int posY);

	void DrawInCamera(int posXMin, int posXMax, int posYMin, int posYMax);

	void Draw();
};

