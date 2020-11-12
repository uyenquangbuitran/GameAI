﻿#include "NPC.h"

#include "SpriteList.h"
#include "GameCollision.h"
#include "GameLog.h"

NPC::NPC()
{
	_type = ET_NPC;
	IsDeleted = false;
	_width = 28;
	_height = 28;

	// khởi tạo các animation
	_leftAnimation = new Animation();
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 82, 82 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	_rightAnimation = new Animation();
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 233, 233 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	_upAnimation = new Animation();
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 2, 2 + 32, 4, 4 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	_downAnimation = new Animation();
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 156, 156 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));

	// mặc định animation ban đầu
	_currentAnimation = _leftAnimation;

	Position = D3DXVECTOR2(15.f, 15.f);
}

void NPC::Update(float _dt)
{
	if (IsDeleted)
		return;

	if (_isPausing)
	{
		onPauseTime += _dt;
		if (onPauseTime >= pauseTime)
		{
			_isPausing = false;
			onPauseTime = 0;
		}			
	}		

	Position += Velocity * _dt;
}

void NPC::Draw()
{
	if (IsDeleted)
		return;

	_currentAnimation->Draw(Position);
}

void NPC::Move(D3DXVECTOR2 destination)
{
	if (abs(destination.x - Position.x) < 2)
	{
		destination.x = round(destination.x);
		Position.x = destination.x;
	}

	if (abs(destination.y - Position.y) < 2)
	{
		destination.y = round(destination.y);
		Position.y = destination.y;
	}

	if (destination.x - Position.x < -0.5f)
	{
		_direction = D_Left;
		_isMoving = true;
	}
	else if (destination.x - Position.x > 0.5f)
	{
		_direction = D_Right;
		_isMoving = true;
	}
	else if (destination.y - Position.y < -0.5f)
	{
		_direction = D_Up;
		_isMoving = true;
	}
	else if (destination.y - Position.y > 0.5f)
	{
		_direction = D_Down;
		_isMoving = true;
	}
	else
	{
		_direction = D_Stand;
		_isMoving = false;
	}		
	
	ApplyVelocity();
	ApplyAnimation();
}

Direction NPC::GetNewDirection()
{	
	if (_direction == D_Stand)
		Move(path[currentNodeIndex]->GetPosition());

	if (changePathTime == 0)
	{
		_oldDirection = _direction;
		currentNodeIndex = 0;
	}		

	Direction d;
	switch (changePathTime)
	{

		if (_direction == D_Stand)
			GAMELOG("ERROR!");
	case 0:
		if (rand() % 2 == 0)
		{
			switch (_direction) // turn to the left of the current direction.
			{
			case D_Left:
				d = D_Down;
				break;
			case D_Up:
				d = D_Left;
				break;
			case D_Right:
				d = D_Up;
				break;
			case D_Down:
				d = D_Right;
				break;
			}
		}
		else
		{
			switch (_direction) // turn to the right of the current direction.
			{
			case D_Left:
				d = D_Up;
				break;
			case D_Up:
				d = D_Right;
				break;
			case D_Right:
				d = D_Down;
				break;
			case D_Down:
				d = D_Left;
				break;
			}
		}		
		break;
	case 1:
		switch (_direction) // turn to the opposite of the new direction.
		{
		case D_Left:
			d = D_Right;
			break;
		case D_Up:
			d = D_Down;
			break;
		case D_Right:
			d = D_Left;
			break;
		case D_Down:
			d = D_Up;
			break;
		}
		break;
	case 2:
		switch (_oldDirection) // turn to the opposite of the old direction.
		{
		case D_Left:
			d = D_Right;
			break;
		case D_Up:
			d = D_Down;
			break;
		case D_Right:
			d = D_Left;
			break;
		case D_Down:
			d = D_Up;
			break;
		}
		break;		
	default:
		d = D_Stand;
		break;
	}

	return d;
}

void NPC::Stop()
{
	_direction = D_Stand;
	_isMoving = false;
	ApplyVelocity();
}

void NPC::Pause(float delayAmount)
{
	pauseTime = delayAmount;
	_direction = D_Stand;
	_isMoving = false;
	_isPausing = true;
	ApplyVelocity();
}

void NPC::ChangePath(GridTile* newPoint)
{
	if (newPoint)
		tempPath.emplace_back(newPoint);
}

void NPC::CheckCollision(Entity * e)
{
	if (IsDeleted)
		return;

	CollisionResult cR = GameCollision::getCollisionResult(this, e);
	if (cR.IsCollided)
	{
		if (cR.Side == CS_Left)
		{
			Position.x += (float)(cR.Rect.right - cR.Rect.left) + 1;
		}
		else if (cR.Side == CS_Right)
		{
			Position.x -= (float)(cR.Rect.right - cR.Rect.left) - 1;
		}
		else if (cR.Side == CS_Top)
		{
			Position.y += (float)(cR.Rect.bottom - cR.Rect.top) + 1;
		}
		else if (cR.Side == CS_Bottom)
		{
			Position.y -= (float)(cR.Rect.bottom - cR.Rect.top) - 1;
		}
	}
}

bool NPC::GetCollisionResult(Entity* e)
{
	if (IsDeleted)
		return false;

	CollisionResult cR = GameCollision::getCollisionResult(this, e);
	if (cR.IsCollided)
	{
		if (cR.Side == CS_Left)
		{
			Position.x += (float)(cR.Rect.right - cR.Rect.left) + 2;
		}
		else if (cR.Side == CS_Right)
		{
			Position.x -= (float)(cR.Rect.right - cR.Rect.left) - 2;
		}
		else if (cR.Side == CS_Top)
		{
			Position.y += (float)(cR.Rect.bottom - cR.Rect.top) + 2;
		}
		else if (cR.Side == CS_Bottom)
		{
			Position.y -= (float)(cR.Rect.bottom - cR.Rect.top) - 2;
		}
	}	

	return cR.IsCollided;
}

void NPC::ApplyAnimation()
{
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
	default:
		break;
	}
}

D3DXVECTOR2 NPC::getVelocityByDirection(Direction direction)
{
	if (direction == D_Stand)
	{
		return D3DXVECTOR2(0, 0);
	}
	else if (direction == D_Left)
	{
		return D3DXVECTOR2(-_speed, 0);
	}
	else if (direction == D_Right)
	{
		return D3DXVECTOR2(_speed, 0);
	}
	else if (direction == D_Up)
	{
		return D3DXVECTOR2(0, -_speed);
	}
	else if (direction == D_Down)
	{
		return D3DXVECTOR2(0, _speed);
	}

	return D3DXVECTOR2(0, 0);
}

void NPC::ApplyVelocity()
{
	switch (_direction)
	{
	case D_Stand:
		Velocity = D3DXVECTOR2(0.f, 0.f);
		break;
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
	}
}
