#pragma once

#include "Entity.h"
#include "Explosion.h"
#include <vector>
#include "GridTile.h"
#include "Bullet.h"
#include <algorithm> 

enum NPCState
{
	NPCS_RunAstar,
	NPCS_RunDodging,
	NPCS_RunAttack,
	NPCS_Attacking,
	NPCS_RunCollect,
	NPCS_RunNoAttack
};

class NPC :public Entity
{
protected:
	float _aggresivePoint = 30.f;
	float _attackRangeFactor = 1.f;
	float _attackRatioFactor = 1.f;

	float _speed = 100.f;
	int _maxHP = 5;
	bool _isShield = false;
	Direction _direction; // hướng di chuyển
	Direction _attackDirection = D_Stand; // lưu hướng tấn công
	Direction _shotDirection = D_Stand; //lưu hướng bắn đạn
	NPCState _state = NPCS_RunAstar; //lưu hướng bắn đạn

	std::vector<Explosion*> _explosionList; // trỏ đến


	Animation* _leftAnimation;
	Animation* _rightAnimation;
	Animation* _upAnimation;
	Animation* _downAnimation;
	Animation *_currentAnimation; // animation hiện tại
	Animation* _shieldAnimation; // animation khiên 	

	void ApplyAnimation();
	D3DXVECTOR2 GetVelocityByDirection(Direction direction);

public:
	std::vector<Bullet*> _bulletList;

	NPC();
	~NPC() {}	
	void Update(float dt) override;
	void Draw() override;
	bool CheckCollision(Entity* entity);
	void ApplyVelocity();
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }
	void SetPosition(float x, float y);
	float GetAggresivePoint() { return _aggresivePoint; }

protected:
	//tọa độ trên astar
	Vector2D* vec = new Vector2D();

	std::vector<GridTile*> path;
	int currentNodeIndex = 0;
	bool _isMoving = true;
	bool _isDodging = false;  //đang né tránh vật cản trên đường
	bool _isCollision = false; //đang có va chạm hay không
	bool _isDivingToPlayer = false;

	float _timeWaittingMove = 0; //đếm thời gian
	Vector2D* _lastDesPos = new Vector2D(); //lưu vị trí cuối path

	float _shieldedTime = 0.f;

	void AutoMove(float dt);
	void Move(D3DXVECTOR2 destination);

	void Stop();


	//khi hết di chuyển tự động auto lại star
	void RunAstar();
	void PathAStar(int posX, int posY);
	void RunDodging(); //di chuyển né
	virtual void RunRandom();

	//dự đoán va chạm
	void PredictCollision();

	//SHOOT
	bool _isAutoShoot;
	float  _timeWaittingShoot;
	float _shotCooldown = 4; 
	int _currentBullet;
	int _attackRange = 8;
	int _attackRatio = 65; //65% tỷ lệ bắn mỗi lần quét
	bool _isEnemyInRange; //player trong tầm nhắm thì đuổi theo bắn player
	Vector2D* vecPlayer = new Vector2D();

	void AutoShoot(float dt); //tự động bắn với tốc độ chậm, => nếu trên đường bắn player thì bắn với tốc độ nhanh

public:
	void SetIsCollision(bool isCollision);

	//ở ngoài set đường đi cho bot
	void MoveGridAstar(int x, int y);

	bool CheckPlayerInRange(int x, int y, bool isCollect = false);
	//nếu player ở trong vùng quét => astar tới chỗ có thể bắn được player
	//chỗ bắn được player tính bằng trục ngang, trục dọc theo gird player
	//di chuyển tới trục ngang, trục dọc không có vật cản để bắn

	void DrawPath();

	void Damage(int _value = 1) override
	{
		if (!_isShield)
		{
			Entity::Damage(_value);
		}
	}
};