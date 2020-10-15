#pragma once
#include "Animation.h"
#include "GameGlobal.h"
#include "GameDefine.h"

class GridTile
{
protected:
	int x, y;
	Animation* anim;
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);
	TileType type;
	D3DCOLOR color;
	friend class GridScene;
public:
	GridTile();
	~GridTile() {};
	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }
	int GetX() { return x; }
	int GetY() { return y; }
	void Draw();
	void SetType(TileType type);
	void SetPosition(D3DXVECTOR2 position) { this->position = position; }
};