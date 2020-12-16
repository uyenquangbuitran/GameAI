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
	if (IsDeleted)
		return;

	Position += Velocity * _dt;
}

void Bullet::CheckCollision(Entity* e)
{
	if (IsDeleted) return;

	CollisionResult cR = GameCollision::getCollisionResult(this, e);
	if (cR.IsCollided)
		MakeCollision(e);
}

void Bullet::MakeCollision(Entity* e)
{
	IsDeleted = true;

	if (dynamic_cast<BrickNormal*>(e))
		e->IsDeleted = true; //Destroy brick.

	if (dynamic_cast<Player*>(e) && e != shotTank)
		e->IsDeleted = true; // Need a function to damage player instead of destroying it.

	if (dynamic_cast<NPC*>(e) && e != shotTank)
		e->IsDeleted = true; // NPC need a damage function as well.
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
	if (IsDeleted)
		return;

	_currentAnimation->Draw(Position);
}

void Bullet::addExpolostion(Explosion* e)
{
	explosionList.emplace_back(e);
}