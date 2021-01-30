#pragma once
#include "Animation.h"
#include "GameGlobal.h"
#include "GameDefine.h"

class GridTile
{
protected:
	Vector2D vec;
	Animation* anim;
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);
	TileType type;
	D3DCOLOR color;
	int currentNode = 0;
	friend class GridScene;
	friend class HorizontalFieldScene;
	friend class BigScene;
public:
	GridTile();
	~GridTile() {};
	void SetVec(Vector2D value) { vec = value; }
	void SetVec(int x, int y) { vec.set(x, y); }
	Vector2D GetVec() { return vec; }
	void Draw();
	TileType GetType() { return type; };
	void SetType(TileType type);
	void SetPosition(D3DXVECTOR2 position) { this->position = position; }
	D3DXVECTOR2 GetPosition() { return position; }
};