#pragma once
#include "NPC.h"

class NPCTanker : public NPC
{
public:
	NPCTanker() : NPC()
	{
		_type = ET_NPC;

		// Slow but strong and durable.
		_aggresivePoint = 35.f;
		_speed = 80.0f;
		_hp = rand() % 3 + 8; //8=> 10
		_maxHP = _hp;
		_attackRange = rand() % 3 + 4; //4 => 6
		_shotCooldown = 6.0f;
		
		_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 387, 387 + 31, D3DXVECTOR2(18.f, 15.f)));
		_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 387, 387 + 31, D3DXVECTOR2(18.f, 15.f)));
		_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 384, 384 + 37, D3DXVECTOR2(16.f, 18.f)));
		_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 387, 387 + 37, D3DXVECTOR2(15.f, 18.f)));
	}
	~NPCTanker() {}
};