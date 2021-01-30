#pragma once
#include "NPC.h"

class PlayerHunter : public NPC
{
public:
	PlayerHunter() : NPC()
	{
		_type = ET_Player;

		// Fast, agressive but fragile.
		_aggresivePoint = 50.f;
		_speed = 150.0f;
		_hp = rand() % 3 + 4;
		_maxHP = _hp;
		_attackRange = rand() % 3 + 3;
		_shotCooldown = 2.0f;

		
		_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
		_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
		_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 4, 4 + 32, D3DXVECTOR2(16.f, 16.f)));
		_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
	}
	~PlayerHunter() {}
};