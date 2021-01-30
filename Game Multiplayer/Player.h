#pragma once

#include "Entity.h"
#include <map>
#include "Bullet.h"
#include <vector>
#include "GameLog.h"
#include "GameDefine.h"
#include "GridTile.h"
#include "AStar.h"

using namespace std;

class Player : public Entity
{
	const float _speed = 130.f;
	Direction _direction = D_Stand;
	Direction _shotDirection = D_Stand;
	bool _isShield = false;
	int _level = 1;
	int _score = 0;
	int _color_Position = 0;
	int _maxHP = 10;

	const float _time_BetweenShoots = 0.7f;

	float _shieldedTime = 0.f;
	float _count_Shoot = 0.0f;
	int _currentBullet;

	float _waittingTime = 0.f;
	float onPauseTime = 0.f;
	float pauseTime = 0.f;
	

	bool _isMoving = true;
	bool _isDodging = false;  // avoiding obstacles on path.
	bool _isCollision = false;
	bool _isRunningAStar = false;

	Vector2D* _lastDestPos = new Vector2D();

	std::vector<Explosion*> _explosionList; // trỏ đến

	Animation* _leftAnimationLv01;
	Animation* _rightAnimationLv01;
	Animation* _upAnimationLv01;
	Animation* _downAnimationLv01;
	Animation* _leftAnimationLv02;
	Animation* _rightAnimationLv02;
	Animation* _upAnimationLv02;
	Animation* _downAnimationLv02;
	Animation* _leftAnimationLv03;
	Animation* _rightAnimationLv03;
	Animation* _upAnimationLv03;
	Animation* _downAnimationLv03;
	Animation* _currentAnimation; // animation hiện tại

	Animation* _arrowAnimation; // animation mũi tên ở trên người chơi
	Animation* _shieldAnimation; // animation khiên 
	Animation* _spawnAnimation; // animation hồi sinh	

	void SetAnimation(Direction _dir);
	void InitAnimation();
	void LogPosition() { GAMELOG("(%i, %i)", (int)Position.x, (int)Position.y); }	

	void MovePath(float dt);
	void RunAStar();
	void RunRandom();
	void RunDodging();
	void PathAStar(int x, int y);
public:
	std::vector<Bullet*> _bulletList;

	Player();
	~Player() {}
	void Update(float dt);
	void Draw();
	void Draw(D3DXVECTOR2 offset);
	void DrawArrow();
	void CheckCollision(Entity* entity);
	void HandleKeyboard(std::map<int, bool> _keys, float _dt);
	void ApplyVelocity(); // vận tốc theo direction
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }
	
	int getScore() { return _score; }	
	std::vector<GridTile*> path;
	int currentNodeIndex = 0;

	bool hasOrder = false;	

	bool IsMoving() { return _isMoving; }

	void Move(D3DXVECTOR2 destination);
	void Stop();
	void SetPosition(float x, float y);
};