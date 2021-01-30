#pragma once
#include "NPC.h"
#include "Eagle.h"
#include "AStar.h"

class PlayerGuard : public NPC
{
public:
	Eagle* _eagle;
	int _range;

	PlayerGuard() : NPC() {}
	PlayerGuard(Eagle* eagle, int range)
	{
		PlayerGuard();
		_type = ET_Player;

		//Guarding a specific point.
		_aggresivePoint = 20.f;
		_speed = 100.0f;
		_hp = rand() % 3 + 2; //2=> 4
		_maxHP = _hp;
		_attackRange = rand() % 4 + 4;
		_shotCooldown = 4.0f;
		
		_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 312, 312 + 32, D3DXVECTOR2(16.f, 16.f)));
		_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		
		_eagle = eagle;
		_range = range;
	}

	void RunRandom()
	{
		if (_eagle->IsDeleted)
		{
			Vector2D* pos = new Vector2D();
			Vector2D eaglePos;
			eaglePos.x = _eagle->Position.x / X_STEP;
			eaglePos.y = _eagle->Position.y / Y_STEP;
			if (AStar::getInstance()->RandomValidPosAround(eaglePos, pos, _range))
				PathAStar(pos->x, pos->y);
		}
		else
		{
			NPC::RunRandom();
		}
	}
};