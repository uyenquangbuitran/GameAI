#include "BigScene.h"
#include "AStar.h"
#include "GameLog.h"
#include "GameDefine.h"
#include "NPCDecisionMaker.h"

void BigScene::InitCamera(GraphicsDevice* gDevice)
{
	camera = new Camera(gDevice, WIDTH, HEIGHT, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	camera->Follow(_player);
	//camera->Follow(_npcList.at(0));
}

BigScene::BigScene()
{
	_map = new GameMap("Resource files/ai_map5.tmx");

	for (int x = 0; x < MAX_X_TILE; x++)
	{
		for (int y = 0; y < MAX_Y_TILE; y++)
		{
			mapGrid[x][y] = new GridTile();
			mapGrid[x][y]->SetVec(x, y);
			mapGrid[x][y]->SetPosition(D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP));
		}
	}

	_waterBrick = new Water(D3DXVECTOR2(10.0f, 10.0f));

	std::vector<Brick*> bricks = _map->getBrickList();
	for (std::vector<Brick*> ::iterator it = bricks.begin(); it != bricks.end(); it = next(it))
	{
		float posX = (*it)->Position.x;
		float posY = (*it)->Position.y;
		//set Obstacle astar
		AStar::getInstance()->SetValue((posX / X_STEP), (posY / Y_STEP), ASTAR_VALUE_BRICK);
	}

	_player = new Player();
	_player->SetPosition(48.f, 3792.f);
	_bulletList.insert(_bulletList.begin(), _player->_bulletList.begin(), _player->_bulletList.end());


	//set gia tri astar cua palyer
	AStar::getInstance()->SetValue(_player->Position.x / X_STEP, _player->Position.y / Y_STEP, ASTAR_VALUE_PLAYER);

	//inint eagle
	eagleList = _map->getEagleList();
	for (auto eagle : eagleList)
	{
		AStar::getInstance()->SetValue((eagle->Position.x / X_STEP), (eagle->Position.y / Y_STEP), ASTAR_VALUE_EAGLE);
		switch (eagle->getType())
		{
		case ET_EagleNPC:
			npcEagle = eagle;
			break;
		case ET_EaglePlayer:
			playerEagle = eagle;
			break;
		default:
			break;
		}
	}

	//tạo other player
	for (int i = 0; i < MAX_PLAYER_HUNTER; i++)
	{
		Vector2D *pos = new Vector2D();
		PlayerHunter *hunter = new PlayerHunter();
		NPC* unit = static_cast<NPC *>(hunter);
		if (AStar::getInstance()->RamdomValidPosition(pos))
		{
			unit->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)*  Y_STEP);
			AStar::getInstance()->SetValue(pos->x, pos->y, ASTAR_VALUE_NPC);
		}
		_playerList.push_back(unit);
		_bulletList.insert(_bulletList.begin(), unit->_bulletList.begin(), unit->_bulletList.end());
	}

	//tạo player security
	for (int i = 0; i < MAX_PLAYER_GUARD; i++)
	{
		Vector2D *pos = new Vector2D();

		Vector2D vecEagle;
		vecEagle.x = playerEagle->Position.x / X_STEP;
		vecEagle.y = playerEagle->Position.y / Y_STEP;

		//chạy xung quanh aegle 4 ô
		PlayerGuard* playerGuard = new PlayerGuard(playerEagle, 4);

		NPC* unit = static_cast<NPC *>(playerGuard);
		if (AStar::getInstance()->RandomValidPosAround(vecEagle, pos, 6))
		{
			unit->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5) * Y_STEP);
			AStar::getInstance()->SetValue(pos->x, pos->y, ASTAR_VALUE_NPC);
		}
		_playerList.push_back(unit);
		_bulletList.insert(_bulletList.begin(), unit->_bulletList.begin(), unit->_bulletList.end());
	}

	// tạo npcs
	for (int i = 0; i < MAX_NPC_HUNTER; i++)
	{
		Vector2D *pos = new Vector2D();
		NPCHunter *hunter = new NPCHunter();
		NPC* npc = static_cast<NPC *>(hunter);
		if (AStar::getInstance()->RamdomValidPosition(pos))
		{
			npc->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5) * Y_STEP);
		}
		_npcList.push_back(npc);
		_bulletList.insert(_bulletList.begin(), npc->_bulletList.begin(), npc->_bulletList.end());
	}

	for (int i = 0; i < MAX_NPC_TANKER; i++)
	{
		Vector2D *pos = new Vector2D();
		NPCTanker *tanker = new NPCTanker();
		NPC* npc = static_cast<NPC *>(tanker);
		if (AStar::getInstance()->RamdomValidPosition(pos))
		{
			npc->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5) * Y_STEP);
		}
		_npcList.push_back(npc);
		_bulletList.insert(_bulletList.begin(), npc->_bulletList.begin(), npc->_bulletList.end());
	}

	//spawn NPC security
	for (int i = 0; i < MAX_NPC_GUARD; i++)
	{
		Vector2D *pos = new Vector2D();
		Vector2D vecEagle;
		vecEagle.x = npcEagle->Position.x / X_STEP;
		vecEagle.y = npcEagle->Position.y / Y_STEP;

		//chạy xung quanh aegle 4 ô
		NPCGuard* npcGuard = new NPCGuard(npcEagle, 4);

		NPC* npc = static_cast<NPC *>(npcGuard);
		if (AStar::getInstance()->RandomValidPosAround(vecEagle, pos, 6))
		{
			npc->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5) * Y_STEP);
			AStar::getInstance()->SetValue(pos->x, pos->y, ASTAR_VALUE_NPC);
		}
		_npcList.push_back(npc);
		_bulletList.insert(_bulletList.begin(), npc->_bulletList.begin(), npc->_bulletList.end());
	}


	// tạo 5 big explosion
	for (int i = 0; i < 5; i++)
	{
		Explosion* e = new Explosion(true);
		_bigExplosionList.push_back(e);

		for (auto npc : _npcList)
		{
			npc->addExplosion(e);
		}

		for (auto aiPlayer : _playerList)
		{
			aiPlayer->addExplosion(e);
		}

		_player->addExplosion(e);
	}

	// tạo 2 loại Protected Item và Upgrade Item
	for (int i = 0; i < MAX_SHIELD_ITEM; i++)
	{
		ProtectItem *_protectItem = new ProtectItem(D3DXVECTOR2(250.0f, 250.0f));
		_protectItem->IsDeleted = true;
		_protectItemList.push_back(_protectItem);
	}

	for (int i = 0; i < MAX_HEAL_ITEM; i++)
	{
		HealItem *_healItem = new HealItem(D3DXVECTOR2(250.0f, 250.0f));
		_healItem->IsDeleted = true;
		_healItemList.push_back(_healItem);
	}
}


void BigScene::Update(float dt)
{
	if (camera)
	{
		camera->Update();
	}

#pragma region GameModeInput
	if (keyboard[0x31]) // 1 -> Aggresive.
	{
		NPCDecisionMaker::GetInstance()->SetMode(STA_Aggresive);
		keyboard[0x31] = false;
	}
	else if (keyboard[0x32]) // 2 -> Normal.
	{
		NPCDecisionMaker::GetInstance()->SetMode(STA_Normal);
		keyboard[0x32] = false;
	}
	else if (keyboard[0x33]) // 3 -> Defensive.
	{
		NPCDecisionMaker::GetInstance()->SetMode(STA_Defensive);
		keyboard[0x33] = false;
	}
#pragma endregion

	if (!_player->IsDeleted)
	{
		for (NPC *npc : _npcList) // players va chạm npcs
		{
			npc->Update(dt);
			if (!npc->IsDeleted)
			{
				bool _onColl = false;
				if (npc->CheckCollision(_player))
				{
					if (_onColl == false)
						_onColl = true;
				}
				for (auto npcOther : _npcList)
				{
					if (npc != npcOther)
					{
						if (npc->CheckCollision(npcOther))
						{
							if (_onColl == false)
								_onColl = true;
						}
					}
				}
				for (auto aiPlayer : _playerList)
				{
					if (npc->CheckCollision(aiPlayer))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check va chạm với brick
				for (auto brick : _map->getBrickListAroundEntity(npc->Position.x / X_STEP, npc->Position.y / Y_STEP))
				{
					if (npc->CheckCollision(brick))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check eagle
				for (auto eagle : eagleList)
				{
					if (npc->CheckCollision(eagle))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}
				//check item
				for (auto item : _protectItemList)
				{
					npc->CheckCollision(item);
				}
				for (auto item : _healItemList)
				{
					npc->CheckCollision(item);
				}

				npc->SetIsCollision(_onColl);

				_player->CheckCollision(npc);


				//quét theo thứ tự: player => player other => player eagle
				bool inRange = false;

				inRange = npc->CheckPlayerInRange(_player->Position.x / X_STEP, _player->Position.y / Y_STEP);

				if (!inRange) {
					for (NPC *aiPlayer : _playerList)
					{
						if (!aiPlayer->IsDeleted) {
							if (npc->CheckPlayerInRange(aiPlayer->Position.x / X_STEP, aiPlayer->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}
					}
				}

				if (!inRange)
				{
					for (Eagle* eagle : eaglePlayerList)
					{
						if (!eagle->IsDeleted)
						{
							if (npc->CheckPlayerInRange(eagle->Position.x / X_STEP, eagle->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}
					}
				}

				if (!inRange)
				{
					for (HealItem* item : _healItemList)
					{
						if (!item->IsDeleted)
						{
							if (npc->CheckPlayerInRange(item->Position.x / X_STEP, item->Position.y / Y_STEP), true)
							{
								inRange = true;
								break;
							}
						}
					}
				}

				if (!inRange)
				{
					for (ProtectItem* item : _protectItemList)
					{
						if (!item->IsDeleted)
						{
							if (npc->CheckPlayerInRange(item->Position.x / X_STEP, item->Position.y / Y_STEP), true)
							{
								inRange = true;
								break;
							}
						}
					}
				}
			}
		}

		//Ai player player
		for (NPC *aiPlayer : _playerList) // players va chạm npcs
		{
			aiPlayer->Update(dt);
			if (!aiPlayer->IsDeleted)
			{
				bool _onColl = false;
				if (aiPlayer->CheckCollision(_player))
				{
					if (_onColl == false)
						_onColl = true;
				}
				for (auto playerOther : _playerList)
				{
					if (aiPlayer != playerOther) {
						if (aiPlayer->CheckCollision(playerOther))
						{
							if (_onColl == false)
								_onColl = true;
						}
					}
				}
				for (auto npc : _npcList)
				{
					if (aiPlayer->CheckCollision(npc))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check va chạm với brick
				for (auto brick : _map->getBrickListAroundEntity(aiPlayer->Position.x / X_STEP,
					aiPlayer->Position.y / Y_STEP))
				{
					if (aiPlayer->CheckCollision(brick))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check eagle
				for (auto eagle : eagleList)
				{
					if (aiPlayer->CheckCollision(eagle))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check item
				for (auto item : _protectItemList)
				{
					aiPlayer->CheckCollision(item);
				}
				for (auto item : _healItemList)
				{
					aiPlayer->CheckCollision(item);
				}

				aiPlayer->SetIsCollision(_onColl);

				_player->CheckCollision(aiPlayer);


				//quét theo thứ tự: npc => npc eagle
				bool inRange = false;
				if (!inRange)
				{
					for (NPC *npc : _npcList)
					{
						if (!npc->IsDeleted)
						{
							if (aiPlayer->CheckPlayerInRange(npc->Position.x / X_STEP, npc->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}
					}
				}
				if (!inRange)
				{
					for (Eagle* eagle : eagleNPCList)
					{
						if (!eagle->IsDeleted)
						{
							if (aiPlayer->CheckPlayerInRange(eagle->Position.x / X_STEP, eagle->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}
					}
				}

				if (!inRange)
				{
					for (HealItem* item : _healItemList)
					{
						if (!item->IsDeleted)
						{
							if (aiPlayer->CheckPlayerInRange(item->Position.x / X_STEP, item->Position.y / Y_STEP), true)
							{
								inRange = true;
								break;
							}
						}
					}
				}

				if (!inRange)
				{
					for (ProtectItem* item : _protectItemList)
					{
						if (!item->IsDeleted)
						{
							if (aiPlayer->CheckPlayerInRange(item->Position.x / X_STEP, item->Position.y / Y_STEP), true)
							{
								inRange = true;
								break;
							}
						}
					}
				}
			}
		}


		//check brick
		for (auto brick : _map->getBrickListAroundEntity(_player->Position.x / X_STEP, _player->Position.y / Y_STEP))
		{
			_player->CheckCollision(brick);
		}

		//check eagle
		for (auto eagle : eagleList)
		{
			_player->CheckCollision(eagle);
		}
		//check item
		for (auto item : _protectItemList)
		{
			_player->CheckCollision(item);
		}
		for (auto item : _healItemList)
		{
			_player->CheckCollision(item);
		}

		_player->HandleKeyboard(keyboard, dt);

		_player->Update(dt);

	}

	for (auto bullet : _bulletList)
	{
		bullet->Update(dt);

		//check va chạm brick xung quang
		for (auto brick : _map->getBrickListAroundEntity(bullet->Position.x / X_STEP, bullet->Position.y / Y_STEP))
		{
			bullet->CheckCollision(brick);
		}
	}

	// update các object còn lại
	{

		for (auto explosion : _smallExList)
		{
			explosion->Update(dt);
		}
		for (auto explosion : _bigExplosionList)
		{
			explosion->Update(dt);
		}

		_timeSpawnProtect += dt;
		if (_timeSpawnProtect > SPAWN_ITEM_DELAY)
		{
			_timeSpawnProtect = 0;
			for (auto protectItem : _protectItemList)
			{
				if (protectItem->IsDeleted)
				{
					protectItem->IsDeleted = false;
					Vector2D *pos = new Vector2D();
					if (AStar::getInstance()->RamdomValidPosition(pos))
					{
						protectItem->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
					}
					break;
				}
			}
		}

		_timeSpawnHeal += dt;
		if (_timeSpawnHeal > SPAWN_ITEM_DELAY)
		{
			_timeSpawnHeal = 0;
			for (auto healItem : _healItemList)
			{
				if (healItem->IsDeleted)
				{
					healItem->IsDeleted = false;
					Vector2D *pos = new Vector2D();
					if (AStar::getInstance()->RamdomValidPosition(pos))
					{
						healItem->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
					}
					break;
				}
			}
		}

		for (auto protectItem : _protectItemList)
		{
			protectItem->Update(dt);
		}
		for (auto healItem : _healItemList)
		{
			healItem->Update(dt);
		}
		//_pointed->Update(dt);
	}
}

void BigScene::Draw()
{
	//chỉ vẽ các objec ở trong camera
	if (!camera) return;

	camera->Draw();
	_map->DrawInCamera((camera->posX - camera->width / 2) / X_STEP, (camera->posX + camera->width / 2) / X_STEP,
		(camera->posY - camera->height / 2) / Y_STEP, (camera->posY + camera->height / 2) / Y_STEP);

	// tránh đường đi nằm phía trên NPC
	if (IS_DRAW_ASTAR_PATH)
	{
		for (NPC *npc : _npcList)
		{
			npc->DrawPath();
		}
	}

	if (IS_DRAW_ASTAR_PATH)
	{
		for (auto aiPlayer : _playerList)
		{
			aiPlayer->DrawPath();
		}
	}

	for (auto npc : _npcList)
	{
		npc->Draw();
	}

	for (auto aiPlayer : _playerList)
	{
		aiPlayer->Draw();
	}

	_player->Draw();

	for (auto bullet : _bulletList)
	{
		bullet->Draw();
	}

	for (auto e : _smallExList)
	{
		e->Draw();
	}

	for (auto e : _bigExplosionList)
	{
		e->Draw();
	}
	for (auto protectItem : _protectItemList)
	{
		protectItem->Draw();
	}
	for (auto healItem : _healItemList)
	{
		healItem->Draw();
	}

	_player->DrawArrow();

	if (!playerEagle->IsDeleted)
		playerEagle->Draw();

	if (!npcEagle->IsDeleted)
		npcEagle->Draw();
}

bool BigScene::RandomGridTileMove(GridTile *grid)
{
	Vector2D *pos = new Vector2D();
	if (AStar::getInstance()->RamdomValidPosition(pos))
	{
		*grid = *mapGrid[pos->x][pos->y];
		return true;
	}
	return false;
}

