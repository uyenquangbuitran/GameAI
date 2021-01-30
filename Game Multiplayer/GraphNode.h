#pragma once

#include <iostream>
#include "GridTile.h"

class Node
{
public:
	int x, y;
	int parentX, parentY;
	float gCost;
	float hCost;
	float fCost;
	int status = 0;
public:
	int GetX() { return x; }
	int GetY() { return y; }
	void SetX(int value) { x = value; }
	void SetY(int value) { y = value; }
	void SetXY(int _x, int _y) { x = _x; y = _y; }
	float GetCost() { return fCost; }
	bool operator < (const Node& n) const;
	bool operator > (const Node& n) const;
	friend std::ostream& operator << (std::ostream& os, const Node& n);
};