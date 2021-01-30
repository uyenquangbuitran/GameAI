#pragma once
#include <array>

#include "GridTile.h"


#include "Scene.h"
#include "GameMap.h"
#include "Player.h"
#include "Bullet.h"
#include "NPC.h"
#include "NPCGuard.h"
#include "NPCHunter.h"
#include "NPCTanker.h"
#include "PlayerHunter.h"
#include "PlayerGuard.h"
#include "Explosion.h"
#include "ProtectItem.h"
#include "UpgradeItem.h"
#include "HealItem.h"
#include "Water.h"
#include "Camera.h"
#include "Astar.h"

class BigScene : public Scene
{
	Camera* camera;

	GameMap* _map;
	Player* _player;

	vector<NPC*> _npcList;
	vector<NPC*> _playerList;

	vector<Bullet*> _bulletList;
	vector<Explosion*> _smallExList;
	vector<Explosion*> _bigExplosionList;

	vector<Eagle*> eagleList;
	vector<Eagle*> eagleNPCList;
	vector<Eagle*> eaglePlayerList;

	Eagle* npcEagle;
	Eagle* playerEagle;

	float _timeSpawnProtect;
	float _timeSpawnHeal;
	vector<ProtectItem*> _protectItemList;
	vector<HealItem*> _healItemList;

	const int MAX_PLAYER_GUARD = 2;
	const int MAX_PLAYER_HUNTER = 7;

	const int MAX_NPC_HUNTER = 12;
	const int MAX_NPC_TANKER = 12;
	const int MAX_NPC_GUARD = 2;

	const int MAX_SHIELD_ITEM = 20;
	const int MAX_HEAL_ITEM = 20;

	Water* _waterBrick;

public:
	void InitCamera(GraphicsDevice* gDevice);

	std::array<std::array<GridTile*, MAX_Y_TILE>, MAX_X_TILE> mapGrid;

	BigScene();
	~BigScene() {}
	void Update(float _dt) override;
	void Draw() override;
	bool RandomGridTileMove(GridTile *grid);
};

