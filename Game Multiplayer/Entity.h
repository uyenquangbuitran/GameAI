#pragma once

#include "GameGlobal.h"
#include "Animation.h"
#include "SpriteList.h"
#include "GameDefine.h"

class Entity
{
protected:
	int _width = 0,
		_height = 0;
	EntityType _type = ET_Unknown; // nhận diện Entity

public:
	D3DXVECTOR2 Position = D3DXVECTOR2(0.f, 0.f);
	D3DXVECTOR2 Velocity = D3DXVECTOR2(0.f, 0.f);
	bool IsDeleted = false;
	int _hp = 3;
public:
	Entity() {}
	~Entity() {}

	virtual void Update(float dt) {}
	virtual void Draw() {}
	virtual void MakeCollision(Entity* _en) {}	// thực hiện va chạm
	virtual bool CanCollision(Entity* _en)
	{
		return (abs(_en->Position.x / X_STEP - Position.x / X_STEP) < 3)
			&& (abs(_en->Position.y / Y_STEP - Position.y / Y_STEP) < 3);
	}

	RECT getBound();

	int getWidth() { return _width; }
	int getHeight() { return _height; }
	EntityType getType() { return _type; }

	virtual void Damage(int value = 1);
};
