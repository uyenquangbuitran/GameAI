#pragma once
#include <vector>
#include <algorithm>

#include "Entity.h"
#include "Explosion.h"
#include "GridTile.h"
#include "Bullet.h"

enum NPCState
{
	NPCS_RunAStar,
	NPCS_RunDodging,
	NPCS_RunAttack,
	NPCS_Attacking,
	NPCS_RunCollect,
	NPCS_RunNoAttack
};

class NPC :public Entity
{
	float _speed = 130.f;
	Direction _direction; // hướng di chuyển
	Direction _faceDirection;
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
	void Draw(D3DXVECTOR2 offset);
	void CheckCollision(Entity* entity);
	bool GetCollisionResult(Entity* entity);
	void ApplyVelocity();
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }

	std::vector<GridTile*> path;
	std::vector<GridTile*> tempPath;
	int currentNodeIndex = 0;
	
	bool hasOrder = false;
	bool hasPath = false;
	bool isChangedPath = false;
	bool isRepath = false;	
	int changePathTime = 0;	

	bool IsMoving() { return _isMoving; }
	bool IsPause() { return _isPausing; }
	bool IsStand() { return (_direction == D_Stand); }
	void Move(D3DXVECTOR2 destination);
	void Stop();
	void Pause(float delayAmount = 0.15f);
	void Fire();

	void ChangePath(GridTile* newPoint);
	Direction GetNewDirection();
};
