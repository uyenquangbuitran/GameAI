#pragma once
#include "GraphNode.h"

class CoorNode
{
	int x, y;	
public:
	CoorNode() { x = 0; y = 0; }
	CoorNode(int _x, int _y) : x(_x), y(_y) {}
	int GetX() const { return x; }
	int GetY() const { return y; }
	bool operator == (const CoorNode& n) const
	{
		return (x == n.x && y == n.y);
	}
};

class PairNode
{
	CoorNode begin;
	CoorNode destination;	
	
public:
	PairNode() { begin = CoorNode(); destination = CoorNode(); }
	PairNode(CoorNode _begin, CoorNode _des) { begin = _begin; destination = _des; }
	CoorNode GetBeginNode() const { return begin; }
	CoorNode GetDestinationNode() const { return destination; }
	bool IsReversed(PairNode& n) const
	{
		return (begin == n.destination && destination == n.begin);
	}

	bool operator== (const PairNode& n) const
	{		
		return ((begin == n.begin && destination == n.destination)
			|| (begin == n.destination && destination == n.begin));
	}
};

namespace std
{
	template <>
	struct hash<PairNode>
	{
		size_t operator()(const PairNode& n) const
		{		
			size_t res = 17;
			res = res * 31 + std::hash<int>()(n.GetBeginNode().GetX());
			res = res * 31 + std::hash<int>()(n.GetBeginNode().GetY());
			res = res * 31 + std::hash<int>()(n.GetDestinationNode().GetX());
			res = res * 31 + std::hash<int>()(n.GetDestinationNode().GetY());

			return res;
		}
	};
}