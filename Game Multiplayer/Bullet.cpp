#include "Bullet.h"

#include "GameCollision.h"

#include "BrickNormal.h"
#include "NPC.h"
#include "Player.h"

Bullet::Bullet()
{
	_type = ET_Bullet;
	IsDeleted = true;
	_width = 6;
	_height = 6;

	_leftAnimation = new Animation();
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 148, 148 + 8, 12, 12 + 6, D3DXVECTOR2(4.f, 3.f)));

	_rightAnimation = new Animation();
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 180, 180 + 8, 12, 12 + 6, D3DXVECTOR2(4.f, 3.f)));

	_upAnimation = new Animation();
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 134, 134 + 6, 12, 12 + 8, D3DXVECTOR2(3.f, 4.f)));

	_downAnimation = new Animation();
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 166, 166 + 6, 12, 12 + 8, D3DXVECTOR2(3.f, 4.f)));

	_currentAnimation = _leftAnimation;
}

void Bullet::Update(float _dt)
{
	_explosion.Update(_dt);

	if (IsDeleted)
		return;

	Position += Velocity * _dt;
}

void Bullet::CheckCollision(Entity* e)
{
	if (e->IsDeleted) return;
	if (IsDeleted) return;

	if (!CanCollision(e)) return;
	if (e->getType() == ET_Bullet) return;

	CollisionResult cR = GameCollision::getCollisionResult(this, e);
	if (cR.IsCollided)
	{
		switch (e->getType())
		{
		case ET_Boundary:			
		case ET_MetalBrick:
			SpawnExplosion();
			break;
		case ET_NormalBrick:
			e->Damage();
			if (e->IsDeleted)
				AStar::getInstance()->SetValue(e->Position.x / X_STEP, e->Position.y / Y_STEP, 0);
			SpawnExplosion();
			break;
		case ET_NPC:
			if (shooterType == ET_Player)
			{
				e->Damage();
				if (e->IsDeleted)
					AStar::getInstance()->SetValue(e->Position.x / X_STEP, e->Position.y / Y_STEP, 0);
				SpawnExplosion();
			}
			break;
		case ET_Player:
			if (shooterType == ET_NPC)
			{
				e->Damage();
				if (e->IsDeleted)
					AStar::getInstance()->SetValue(e->Position.x / X_STEP, e->Position.y / Y_STEP, 0);
				SpawnExplosion();
			}
			break;
		case ET_EaglePlayer:
			if (shooterType == ET_NPC)
			{
				e->Damage();
				if (e->IsDeleted)
					AStar::getInstance()->SetValue(e->Position.x / X_STEP, e->Position.y / Y_STEP, 0);
				SpawnExplosion();
			}
			break;
		case ET_EagleNPC:
			if (shooterType == ET_Player)
			{
				e->Damage();
				if (e->IsDeleted)
					AStar::getInstance()->SetValue(e->Position.x / X_STEP, e->Position.y / Y_STEP, 0);
				SpawnExplosion();
			}
			break;
		default:
			break;
		}
	}
}

void Bullet::MakeCollision(Entity* e)
{
	IsDeleted = true;
}

void Bullet::setDirection(Direction _dir)
{
	_direction = _dir;
	switch (_direction)
	{
	case D_Left:
		_currentAnimation = _leftAnimation;
		break;
	case D_Right:
		_currentAnimation = _rightAnimation;
		break;
	case D_Up:
		_currentAnimation = _upAnimation;
		break;
	case D_Down:
		_currentAnimation = _downAnimation;
		break;
	}
	ApplyVelocity();
}

void Bullet::ApplyVelocity()
{
	if (IsDeleted)
		return;

	switch (_direction)
	{
	case D_Left:
		Velocity = D3DXVECTOR2(-_speed, 0.f);
		break;
	case D_Right:
		Velocity = D3DXVECTOR2(_speed, 0.f);
		break;
	case D_Up:
		Velocity = D3DXVECTOR2(0.f, -_speed);
		break;
	case D_Down:
		Velocity = D3DXVECTOR2(0.f, _speed);
		break;
	default:
		break;
	}
}

void Bullet::Draw()
{
	_explosion.Draw();	
	if (IsDeleted) return;
	_currentAnimation->Draw(Position);
}

void Bullet::Draw(D3DXVECTOR2 offset)
{
	_explosion.Draw();
	if (IsDeleted) return;
	_currentAnimation->Draw(Position, offset);
}

void Bullet::Shoot(D3DXVECTOR2 pos, Direction dir, EntityType _shooterType)
{
	Position = pos;
	setDirection(dir);
	IsDeleted = false;
	shooterType = _shooterType;
	ApplyVelocity();
}
