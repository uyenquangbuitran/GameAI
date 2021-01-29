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
#include "Camera.h"

class BigScene : public Scene
{
	GameMap* _map;	
	std::array<std::array<GridTile*, (MAP_HEIGHT / Y_STEP)>, (MAP_WIDTH / X_STEP)> gridMap;
	std::vector<NPC*> _npcList;
	//std::vector<vector<Bullet*>> _bulletList;
	std::vector<Bullet*> _bulletList;
	std::set<int, std::greater <int>> tankNodes;

	Camera camera;
	D3DXVECTOR2 cameraOffset;

	Player* _player;
	bool _isPlayerMoving = false;
	bool _isMouseActive = true;

	void UpdateTankNodes();
	void FindPlayerPath(Node begin, Node destination);
	void FindPath(Node begin, Node destination);	

public:
	BigScene();
	~BigScene() {};

	void Update(float dt) override;
	void Draw() override;

	void OnLeftMouseDown(float x, float y) override;
	void OnRightMouseDown(float x, float y) override;

	bool IsValidClickPosition(int x, int y) { return _map->IsValidTile(x, y); }

	Bullet* GetBullet();
};