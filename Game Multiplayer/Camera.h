#pragma once
#include <d3dx9.h>
#include <d3d9.h>

class Camera
{
	static D3DXVECTOR2 Position;
	static int ScreenHeight;
	static int ScreenWidth;
	static int LeftLimit;
	static int RightLimit;
	static int TopLimit;
	static int BottomLimit;

	static void Init(int _height, int _width, int _left, int _right, int _top, int _bottom)
	{
		ScreenHeight = _height;
		ScreenWidth = _width;
		LeftLimit = _left;
		RightLimit = _right;
		TopLimit = _top;
		BottomLimit = _bottom;
	}

	static void setPosition(const D3DXVECTOR2& _position)
	{
		Position = _position;

		if (Position.x < LeftLimit + ScreenWidth / 2.f)
		{
			Position.x = LeftLimit + ScreenWidth / 2.f;
		}
		else if (Position.x > RightLimit - ScreenWidth / 2.f)
		{
			Position.x = RightLimit - ScreenWidth / 2.f;
		}

		if (Position.y < TopLimit + ScreenHeight / 2.f)
		{
			Position.y = TopLimit + ScreenHeight / 2.f;
		}
		else if (Position.y > BottomLimit - ScreenHeight / 2.f)
		{
			Position.y = BottomLimit - ScreenHeight / 2.f;
		}
	}

	static RECT getBound()
	{
		RECT _rect;
		_rect.left = Position.x - ScreenWidth / 2.f;
		_rect.right = Position.x + ScreenWidth / 2.f;
		_rect.top = Position.y - ScreenHeight / 2.f;
		_rect.bottom = Position.y + ScreenHeight / 2.f;

		return _rect;
	}
};