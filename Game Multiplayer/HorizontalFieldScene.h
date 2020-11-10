#pragma once

#include <array>
#include <vector>
#include "Scene.h"

#include "GameDefine.h"
#include "SpriteList.h"
#include "GameGlobal.h"
#include "GameLog.h"
#include "GraphNode.h"

#include "Player.h"
#include "NPC.h"

#include "GameMap.h"
#include "Water.h"


class HorizontalFieldScene : public Scene
{	
	GameMap* _map;
	vector<Water*> _waterList;
	vector<NPC*> _npcList;	

	Player* _player;
	int currentNodeIndex = 0;
	bool _isPlayerMoving = false;
	bool _isMouseActive = true;
	int mode = 1;

	GridTile begin, destination;
	std::array<std::array<GridTile*, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> gridMap;
	std::vector<GridTile*> drawingPath;
	std::vector<GridTile*> drawingObstacles;
	std::vector<GridTile*> drawingTankNodes;
	std::set<int, std::greater <int>> npcsDestination;
	GridTile* destinationNode;

	bool _isDrawPath = true;
	bool _isDrawMap = true;
	bool _isDrawObstacles = true;

	void ResetScene();
	void RunAStar();
	void RunAStar(NPC* npc, Node destination);
	void DrawPath();
	void GivePlayerOrder();
	void GiveNPCOrder();
	void GiveNPCPath(NPC* npc);
	NPC* FindChangePathNPC(NPC* npc1, NPC* npc2);	
	GridTile* GetNPCTempPoint(NPC* npc, Direction direction);
	void UpdateTankNodes();
	bool IsValidNPCDestination(int x, int y);
public:
	std::set<int, std::greater <int>> tankNodes;

	HorizontalFieldScene();
	~HorizontalFieldScene() {};

	void Update(float dt) override;
	void Draw() override;

	void OnLeftMouseDown(float x, float y) override;
	void OnRightMouseDown(float x, float  y) override;
};