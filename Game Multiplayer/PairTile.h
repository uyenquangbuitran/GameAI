#pragma once
#include "MapTile.h"
#include "PairNodes.h"

class GateTile : public CoorNode
{
	int cost;
public:
	int GetCost() { return cost; }
	int SetCost(int value) { cost = value; }
	GateTile() : CoorNode() {};
	GateTile(int x, int y) : CoorNode(x, y) {}
};

class PairTile
{
	GateTile begin;
	GateTile destination;
	
public:
	int cost;

	PairTile() { begin = GateTile(); destination = GateTile(); }
	PairTile(GateTile _begin, GateTile _des) { begin = _begin; destination = _des; }
	GateTile GetBeginTile() const { return begin; }
	GateTile GetDestinationTIle() const { return destination; }
	bool IsReversed(PairTile& t) const
	{
		return (begin == t.destination && destination == t.begin);
	}

	bool operator== (const PairTile& t) const
	{
		return ((begin == t.begin && destination == t.destination)
			|| (begin == t.destination && destination == t.begin));
	}
};