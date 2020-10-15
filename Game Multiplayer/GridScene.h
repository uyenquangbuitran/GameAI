#pragma once

#include <array>
#include <vector>
#include "Scene.h"

#include "GameDefine.h"
#include "SpriteList.h"
#include "GameGlobal.h"
#include "AStar.h"

class MovingTile : public GridTile
{		
public:
	bool isAbleToMove = true;
	MovingTile() : GridTile() { SetType(Moving); x = y = 0; isAbleToMove = true; }
	void HandleKeyboard(std::map<int, bool> keys);	
};

class GridScene : public Scene
{	
	MovingTile* movingTile;
	
public:
	GridScene();
	~GridScene() {}
	void Update(float dt) override;
	void Draw() override;

	GridTile begin, destination;
	std::array<std::array<GridTile*, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> map;
	std::vector<GridTile*> path;
};