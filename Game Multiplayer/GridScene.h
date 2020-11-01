#pragma once

#include <array>
#include <vector>
#include "Scene.h"

#include "GameDefine.h"
#include "SpriteList.h"
#include "GameGlobal.h"
#include "AStar.h"
#include "GameLog.h"
#include "Player.h"

class GridScene : public Scene
{	
	//MovingTile* movingTile;
	Player* player;
	int currentNodeIndex = 0;
	bool _isPlayerMoving = false;
	bool _isMouseActive = true;
	int mode = 1;

	GridTile begin, destination;
	std::array<std::array<GridTile*, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> map;
	std::vector<GridTile*> path;
	std::vector<GridTile*> drawPath;

	void ResetScene();
	void RunAStar();
	void DrawPath();
	void GivePlayerOrder();
public:
	GridScene();
	~GridScene() {}
	void Update(float dt) override;
	void Draw() override;

	void OnLeftMouseDown(float x, float y) override;
	void OnRightMouseDown(float x, float  y) override;	
};