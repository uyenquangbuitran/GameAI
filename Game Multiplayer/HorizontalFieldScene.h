#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include "Scene.h"

#include "GameDefine.h"
#include "SpriteList.h"
#include "GameGlobal.h"
#include "GameLog.h"

#include "GraphNode.h"
#include "PairNodes.h"

#include "Player.h"
#include "NPC.h"
#include "Bullet.h"

#include "GameMap.h"
#include "Water.h"

#include "Camera.h"


class HorizontalFieldScene : public Scene
{	
	GameMap* _map;
	vector<Water*> _waterList;
	vector<NPC*> _npcList;	
	vector<Bullet*> _bulletList;

	Camera camera;

	Player* _player;	
	bool _isPlayerMoving = false;
	bool _isMouseActive = true;	

	GridTile begin, destination;
	std::unordered_map<PairNode, std::vector<GridTile*>> storedPath;
	std::array<std::array<GridTile*, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> gridMap;
	std::vector<GridTile*> drawingPath;
	std::vector<GridTile*> drawingObstacles;
	std::vector<GridTile*> drawingTankNodes;
	std::set<int, std::greater <int>> npcsDestination;
	GridTile* destinationNode;

	bool _isDrawPath = true;
	bool _isDrawMap = true;
	bool _isDrawDestinationOnly = false;
	bool _isDrawObstacles = true;

	void ResetScene();
	void FindPath();
	void FindPath(NPC* npc, Node destination);
	void RunAStar();
	void RunAStar(NPC* npc, Node begin, Node destination);
	void DrawPath();
	void GivePlayerOrder();
	void GiveNPCPath(NPC* npc);
	NPC* FindChangePathNPC(NPC* npc1, NPC* npc2);	
	GridTile* GetNPCTempPoint(NPC* npc, Direction direction);		
	
public:
	std::set<int, std::greater <int>> tankNodes;	

	HorizontalFieldScene();
	~HorizontalFieldScene() {};

	void Update(float dt) override;
	void Draw() override;

	void OnLeftMouseDown(float x, float y) override;
	void OnRightMouseDown(float x, float  y) override;

	void UpdateTankNodes();
	bool IsValidNPCDestination(int x, int y);

	Bullet* GetBullet();
};