#pragma once

#include "Entity.h"
#include "Explosion.h"
#include "GridTile.h"

class NPC :public Entity
{
	float _speed = 130.f;
	Direction _direction; // hướng di chuyển
	Direction _oldDirection;

	float onPauseTime = 0.f;
	float pauseTime = 0.f;

	std::vector<Explosion*> _explosionList; // trỏ đến

	bool _isMoving = false;
	bool _isStopped = false;
	bool _isPausing = false;
	

	Animation* _leftAnimation;
	Animation* _rightAnimation;
	Animation* _upAnimation;
	Animation* _downAnimation;
	Animation *_currentAnimation; // animation hiện tại

	void ApplyAnimation();	
	D3DXVECTOR2 getVelocityByDirection(Direction direction);

public:
	NPC();
	~NPC() {}
	void Update(float dt) override;
	void Draw() override;
	void CheckCollision(Entity* entity);
	bool GetCollisionResult(Entity* entity);
	void ApplyVelocity();
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }

	std::vector<GridTile*> path;
	std::vector<GridTile*> tempPath;
	int currentNodeIndex = 0;
	
	bool hasPath = false;
	bool isChangedPath = false;
	bool isRepath = false;	
	int changePathTime = 0;	

	bool IsMoving() { return _isMoving; }
	bool IsPause() { return _isPausing; }
	void Move(D3DXVECTOR2 destination);
	void Stop();
	void Pause(float delayAmount = 0.15f);

	void ChangePath(GridTile* newPoint);
	Direction GetNewDirection();
};
