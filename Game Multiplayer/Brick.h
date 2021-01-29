#pragma once

#include "Entity.h"
#include "Animation.h"
#include "SpriteList.h"

class Brick : public Entity
{
protected:
	Animation* _animation;

public:
	Brick() {}
	~Brick()  { delete _animation; }

	void Draw(D3DXVECTOR2 offset) {
		if (!IsDeleted)
			_animation->Draw(Position, offset);
	}

// các hàm hỗ trợ
protected:
	void BaseInit(D3DXVECTOR2 position)
	{
		Position = position;
		_width = 32;
		_height = 32;
		_animation = new Animation();
	}
};

