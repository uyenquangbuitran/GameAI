#pragma once

#include "Entity.h"
#include "Explosion.h"

class Bullet : public Entity
{
	const float _speed = 400.f;
	Direction _direction;	// hướng bay

	std::vector<Explosion*> explosionList; // trỏ đến
	
	Animation* _leftAnimation;
	Animation* _rightAnimation;
	Animation* _upAnimation;
	Animation* _downAnimation;
	Animation* _currentAnimation; // trỏ đến 1 trong 4 animation để vẽ lên

	Entity* shotTank;
public:
	Bullet();	
	~Bullet() { }

	void Update(float _dt) override;

	void CheckCollision(Entity* e);
	void MakeCollision(Entity* _en) override;
	// thay đổi vận tốc và animation đựa theo hướng bay
	void setDirection(Direction _dir);
	void ApplyVelocity();

	void Draw() override;
	void Draw(D3DXVECTOR2 offset);

	void addExpolostion(Explosion* e);
	void SetShotTank(Entity* e) { shotTank = e; }
};

