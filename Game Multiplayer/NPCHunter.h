#pragma once
#include "NPC.h"

class NPCHunter : public NPC
{
public:
	NPCHunter() : NPC()
	{
		_type = ET_NPC;

		// Fast, agressive but fragile.
		_aggresivePoint = 50.f;
		_speed = 150.0f;
		_hp = rand() % 3 + 4; // 4 -> 6
		_maxHP = _hp;
		_attackRange = rand() % 5 + 8; // 10 -> 14
		_shotCooldown = 2.0f;
		
		_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 504, 504 + 32, D3DXVECTOR2(18.f, 16.f)));
		_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 502, 502 + 30, D3DXVECTOR2(18.f, 15.f)));
		_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 500, 500 + 36, D3DXVECTOR2(16.f, 18.f)));
		_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 502, 502 + 34, D3DXVECTOR2(15.f, 17.f)));
	}
	~NPCHunter() {};	
};