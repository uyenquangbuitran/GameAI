#pragma once

#include "GameGlobal.h"
#include <vector>
#include <map>
#include "Label.h"
#include "GameDefine.h"

class Scene
{
protected:
	D3DCOLOR backgroundColor;
	std::map<int, bool> keyboard;

public:
	Scene()
	{
		backgroundColor = D3DCOLOR_XRGB(0, 0, 0);
	}
	~Scene() {}

	virtual void Update(float _dt) {}
	virtual void Draw() {}

	virtual void OnKeyDown(int _keyCode)	
	{
		keyboard[_keyCode] = true; 
	}

	virtual void OnKeyUp(int _keyCode)
	{
		keyboard[_keyCode] = false;
	}

	virtual void OnLeftMouseDown(float x, float y)
	{

	}

	D3DCOLOR GetBackcolor()
	{
		return backgroundColor;
	}
};

