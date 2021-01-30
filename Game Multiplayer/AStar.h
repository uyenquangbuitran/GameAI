#pragma region OldAStar
//enum TileDirection
//{
//	TD_Left_Top,
//	TD_Right_Top,
//	TD_Left_Bottom,
//	TD_Right_Bottom
//};
//
//class TilePart
//{
//public:
//	Vector2D vec;
//	TileDirection dir;
//public:
//	std::array < std::array<int, Y_TILE_COUNT>, X_TILE_COUNT> map;
//	std::array<std::array<GridTile*, (Y_TILE_COUNT)>, (X_TILE_COUNT)> gridMap;
//
//
//	static AStar* GetInstance();
//	void Init();
//
//	bool IsValid(int x, int y)
//	{	
//		if (x < 0 || y < 0 || x >= X_TILE_COUNT || y >= Y_TILE_COUNT)
//			return false;
//		return true;
//	}
//
//	bool IsObstacle(int x, int y, std::array < std::array<int, Y_TILE_COUNT>, X_TILE_COUNT> map)
//	{
//		if (IsValid(x, y))
//			return map[x][y] != 0;
//		return false;
//	}
//
//	bool IsValidAndNotObstacle(int x, int y)
//	{
//		return IsValid(x, y) && !IsObstacle(x, y, map);
//	}
//
//	bool IsDestination(int x, int y, Node dest)
//	{
//		if (x == dest.x && y == dest.y)
//			return true;
//		return false;
//	}
//
//	double CalculateH(int x, int y, Node dest)
//	{
//		return (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y)));
//	}
//
//	std::vector<Node> MakePath(std::array<std::array<Node, Y_TILE_COUNT>, X_TILE_COUNT> map, Node dest)
//	{
//		try
//		{
//			//GAMELOG("Found a path!");
//			int x = dest.x;
//			int y = dest.y;
//			dest.fCost = 0;
//			std::stack<Node> path;
//			std::vector<Node> validPath;
//			while (!(map[x][y].parentX == x && map[x][y].parentY == y)
//				&& map[x][y].x != -1 && map[x][y].y != -1)
//			{
//				path.push(map[x][y]);
//				int tempX = map[x][y].parentX;
//				int tempY = map[x][y].parentY;
//				x = tempX;
//				y = tempY;
//			}
//			path.push(map[x][y]);
//
//			while (!path.empty())
//			{
//				Node top = path.top();
//				path.pop();
//				validPath.emplace_back(top);
//			}
//
//			return validPath;
//		}
//		catch (const std::exception& e)
//		{
//			GAMELOG(e.what());
//		}
//	}
//
//	TilePart GetNextPart(Vector2D beginPart, Vector2D destPart)
//	{
//		TilePart part;
//		//Begin tile as the origin.
//		int dx = destPart.x - beginPart.x;
//		int dy = destPart.y - beginPart.y;
//
//		float absTan = (dx == 0) ? dy : abs(dy / dx);
//
//		if (dx > 0)
//		{
//			if (dy > 0)
//			{
//				part.dir = TD_Left_Top;
//				if (absTan <= 0.33f) //[x + 1, y]
//				{
//					part.vec.x = beginPart.x + 1;
//					part.vec.y = beginPart.y;
//				}
//				else if (absTan <= 3) // [x + 1, y + 1]
//				{
//					part.vec.x = beginPart.x + 1;
//					part.vec.y = beginPart.y + 1;
//				}
//				else // [x, y + 1]
//				{
//					part.vec.x = beginPart.x;
//					part.vec.y = beginPart.y + 1;
//				}
//			}
//			else
//			{
//				part.dir = TD_Left_Bottom;
//				if (absTan <= 0.33f) //[x + 1, y]
//				{
//					part.vec.x = beginPart.x + 1;
//					part.vec.y = beginPart.y;
//				}
//				else if (absTan <= 3) // [x + 1, y - 1]
//				{
//					part.vec.x = beginPart.x + 1;
//					part.vec.y = beginPart.y - 1;
//				}
//				else // [x, y - 1]
//				{
//					part.vec.x = beginPart.x;
//					part.vec.y = beginPart.y - 1;				
//				}
//			}
//		}
//		else
//		{
//			if (dy > 0)
//			{
//				part.dir = TD_Right_Top;
//				if (absTan <= 0.33f) //[x - 1, y]
//				{
//					part.vec.x = beginPart.x - 1;
//					part.vec.y = beginPart.y;
//				}
//				else if (absTan <= 3) // [x - 1, y + 1]
//				{
//					part.vec.x = beginPart.x - 1;
//					part.vec.y = beginPart.y + 1;
//				}
//				else // [x, y + 1]
//				{
//					part.vec.x = beginPart.x;
//					part.vec.y = beginPart.y + 1;
//				}
//			}
//			else
//			{
//				part.dir = TD_Right_Bottom;
//				if (absTan <= 0.33f) //[x - 1, y]
//				{
//					part.vec.x = beginPart.x - 1;
//					part.vec.y = beginPart.y;
//				}
//				else if (absTan <= 3) // [x - 1, y - 1]
//				{
//					part.vec.x = beginPart.x - 1;
//					part.vec.y = beginPart.y - 1;
//				}
//				else // [x, y - 1]
//				{
//					part.vec.x = beginPart.x;
//					part.vec.y = beginPart.y - 1;
//				}
//			}
//		}
//
//		return part;
//	}
//
//	std::vector<Node> aStar(Node begin, Node dest, int partSize)
//	{	
//	
//		Vector2D beginPart, destPart;
//		beginPart.x = begin.x / partSize;
//		beginPart.y = begin.y / partSize;
//		destPart.x = dest.x / partSize;
//		destPart.y = dest.y / partSize;
//
//		if (abs(begin.x - dest.x) <= partSize && abs(begin.y - dest.y) <= partSize)
//		{
//			return aStarWithPart(begin, dest, beginPart, destPart, partSize);
//		}
//
//		if (beginPart.x == destPart.x && beginPart.y == destPart.y)
//		{
//			return aStarWithPart(begin, dest, beginPart, destPart, partSize);
//		}			
//		else
//		{
//			Node next;
//			TilePart nextPart = GetNextPart(beginPart, destPart);
//
//			switch (nextPart.dir)
//			{
//			case TD_Left_Top:
//				for (int counter = 0; counter <= partSize; counter++)
//				{
//					for (int x = nextPart.vec.x * partSize; x <= nextPart.vec.x * partSize + counter; x++)
//					{
//						for (int y = nextPart.vec.y * partSize + counter; y >= nextPart.vec.y * partSize; y--)
//						{
//							if (IsValidAndNotObstacle(x, y))
//							{
//								next.x = x;
//								next.y = y;
//								//return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
//							}
//						}
//					}
//				}
//
//			case TD_Left_Bottom:
//				for (int counter = 0; counter <= partSize; counter++)
//				{
//					for (int x = nextPart.vec.x * partSize + counter; x >= nextPart.vec.x * partSize; x--)
//					{
//						for (int y = nextPart.vec.y * partSize; y <= nextPart.vec.y * partSize + counter; y++)
//						{
//							if (IsValidAndNotObstacle(x, y))
//							{
//								next.x = x;
//								next.y = y;
//								//return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
//							}
//						}
//					}
//				}
//				
//			case TD_Right_Top:
//				for (int counter = 0; counter <= partSize; counter++)
//				{
//					for (int x = nextPart.vec.x * partSize - counter; x <= nextPart.vec.x * partSize; x++)
//					{
//						for (int y = nextPart.vec.y * partSize; y <= nextPart.vec.y * partSize + counter; y++)
//						{
//							if (IsValidAndNotObstacle(x, y))
//							{
//								next.x = x;
//								next.y = y;
//								//return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
//							}
//						}
//					}
//				}
//
//			case TD_Right_Bottom:
//				for (int counter = 0; counter <= partSize; counter++)
//				{
//					for (int x = nextPart.vec.x * partSize; x >= nextPart.vec.x * partSize - counter; x--)
//					{
//						for (int y = nextPart.vec.y * partSize - counter; y <= nextPart.vec.y * partSize + counter; y++)
//						{
//							if (IsValidAndNotObstacle(x, y))
//							{
//								next.x = x;
//								next.y = y;
//								//return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
//							}
//						}
//					}
//				}
//			default:
//				break;
//			}
//
//			//return aStar(begin, dest, partSize + PART_SIZE);
//		}		
//	}
//
//	std::vector<Node> aStarWithPart(Node begin, Node dest, Vector2D beginPart, Vector2D destPart, int partSize)
//	{
//		std::vector<Node> empty;
//		if (!IsValid(dest.x, dest.y))
//		{
//			GAMELOG("Invalid destination!");
//			return empty;
//		}
//
//		if (IsObstacle(dest.x, dest.y, map))
//		{
//			GAMELOG("Destination is an obstacle!");
//			return empty;
//		}
//
//		if (IsDestination(begin.x, begin.y, dest))
//		{
//			GAMELOG("Destination reached!");
//			return empty;
//		}
//
//		int xMin, xMax, yMin, yMax;
//
//		if (beginPart.x < destPart.x) {
//			xMin = beginPart.x * partSize;
//			xMax = (destPart.x + 1)*partSize;
//		}
//		else
//		{
//			xMin = destPart.x * partSize;
//			xMax = (beginPart.x + 1) * partSize;
//		}
//
//		if (beginPart.y < destPart.y) {
//			yMin = beginPart.y * partSize;
//			yMax = (destPart.y + 1) * partSize;
//		}
//		else
//		{
//			yMin = destPart.y * partSize;
//			yMax = (beginPart.y + 1) * partSize;
//		}
//
//		if (xMax > X_TILE_COUNT) xMax = X_TILE_COUNT;
//		if (yMax > Y_TILE_COUNT) yMax = Y_TILE_COUNT;
//
//		//Initalize map.
//		std::array<std::array<Node, Y_TILE_COUNT>, X_TILE_COUNT> allMap;
//		bool closedList[Y_TILE_COUNT][X_TILE_COUNT];
//
//		while (partSize < Y_TILE_COUNT / 2)
//		{
//			
//			for (int x = xMin; x < xMax; x++) {
//				for (int y = yMin; y < yMax; y++) {
//					allMap[x][y].fCost = FLT_MAX;
//					allMap[x][y].gCost = FLT_MAX;
//					allMap[x][y].hCost = FLT_MAX;
//					allMap[x][y].parentX = -1;
//					allMap[x][y].parentY = -1;
//					allMap[x][y].x = x;
//					allMap[x][y].y = y;
//
//					closedList[x][y] = false;
//				}
//			}
//
//			//Initialize starting list.
//			int x = begin.x;
//			int y = begin.y;
//			allMap[x][y].fCost = 0.0;
//			allMap[x][y].gCost = 0.0;
//			allMap[x][y].hCost = 0.0;
//			allMap[x][y].parentX = x;
//			allMap[x][y].parentY = y;			
//			bool destinationFound = false;
//
//			FibonacciHeap<Node> openList;
//			openList.insert(allMap[x][y]);
//
//			while (!openList.isEmpty() && openList.size() < (X_TILE_COUNT * Y_TILE_COUNT))
//			{
//				Node node;
//				do
//					node = openList.removeMinimum();
//				while (!openList.isEmpty() && !IsValidAndNotObstacle(node.x, node.y));
//
//				x = node.x;
//				y = node.y;
//				closedList[x][y] = true;
//
//				for (int newX = -1; newX <= 1; newX++)
//				{
//					for (int newY = -1; newY <= 1; newY++)
//					{
//						if (x + newX < xMin || x + newX > xMax
//							|| y + newY < yMin || y + newY > yMax) continue;
//
//						double gNew, hNew, fNew;
//						if (IsValid(x + newX, y + newY))
//						{
//							if (IsDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0))
//							{
//								//Destination found - make path.
//								allMap[x + newX][y + newY].parentX = x;
//								allMap[x + newX][y + newY].parentY = y;
//								destinationFound = true;								
//								return MakePath(allMap, dest);
//							}
//							else if (!closedList[x + newX][y + newY])
//							{
//								if (newX == 0 || newY == 0)
//									gNew = node.gCost + 4.5f;
//								else									
//									continue;
//								hNew = CalculateH(x + newX, y + newY, dest);
//								fNew = gNew + hNew;
//
//								//Check if this path is better than the one already present.
//								if (allMap[x + newX][y + newY].fCost == FLT_MAX || allMap[x + newX][y + newY].fCost > fNew)
//								{
//									allMap[x + newX][y + newY].fCost = fNew;
//									allMap[x + newX][y + newY].gCost = gNew;
//									allMap[x + newX][y + newY].hCost = hNew;
//									allMap[x + newX][y + newY].parentX = x;
//									allMap[x + newX][y + newY].parentY = y;
//									openList.insert(allMap[x + newX][y + newY]);
//								}
//							}
//						}
//					}
//				}
//			}
//
//		}
//	}
//
//	std::vector<Vector2D> GetListVecInAxisValid(int x, int y, int xRound, int yRound, int countBrick)
//	{
//		try
//		{
//			std::vector<Vector2D> validList;
//
//			int _countBrick = 0;
//
//			int dx = xRound > x ? 1 : -1;
//			while (xRound != x && _countBrick <= countBrick)
//			{
//				if (IsValid(x, y))
//				{
//					if (!IsObstacle(x, y, map))
//						validList.emplace_back(*new Vector2D(x, y));
//					else
//						_countBrick++;
//				}
//				x += dx;
//			}
//
//			_countBrick = 0;
//			int dy = yRound > y ? 1 : -1;
//			while (yRound != y && _countBrick <= countBrick)
//			{
//				if (IsValid(x, y))
//				{
//					if (!IsObstacle(x, y, map))
//						validList.emplace_back(*new Vector2D(x, y));
//					else
//						_countBrick++;
//				}
//				y += dy;
//			}
//
//			if (validList.empty())
//				return GetListVecInAxisValid(x, y, xRound, yRound, countBrick + 1);
//
//			return validList;
//			
//		}
//		catch (exception e)
//		{
//			GAMELOG(e.what());
//		}
//	}
//
//	bool RandomValidPostionAround(Vector2D pos0, Vector2D* pos, int range)
//	{
//		std::vector<Vector2D> validPositionList;
//
//		int xMin = pos0.x - range;
//		int xMax = pos0.x + range;
//		int yMin = pos0.y - range;
//		int yMax = pos0.y + range;
//
//		xMin = xMin < 0 ? 0 : xMin;
//		xMax = xMax > X_TILE_COUNT ? X_TILE_COUNT : xMax;
//		yMin = yMin < 0 ? 0 : yMin;
//		yMax = yMax > Y_TILE_COUNT ? Y_TILE_COUNT : yMax;
//
//		for (int x = xMin; x < xMax; x++)
//		{
//			for (int y = yMin; y < yMax; y++)
//			{
//				if (map[x][y] == 0)
//					validPositionList.push_back(*new Vector2D(x, y));
//			}
//		}
//		if (!validPositionList.empty())
//		{
//			int index = rand() % validPositionList.size();
//			*pos = validPositionList[index];
//			return true;
//		}
//		else GAMELOG("No valid empty postion!");
//
//		return false;
//	}
//
//	bool RandomValidPostion(Vector2D *position)
//	{
//		int partX = rand() % (X_TILE_COUNT / PART_SIZE);
//		int partY = rand() % (Y_TILE_COUNT / PART_SIZE);
//
//		Vector2D partPosition;
//		partPosition.x = (partX + 0.5f) * PART_SIZE;
//		partPosition.y = (partY + 0.5f) * PART_SIZE;
//
//		return RandomValidPostionAround(partPosition, position, PART_SIZE);
//	}
//
//	void SetTileValue(int x, int y, int value)
//	{
//		if (IsValid(x, y)) map[x][y] = value;
//	}	
//};
#pragma endregion

#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "GameDefine.h"
#include "GameLog.h"
#include "GraphNode.h"
#include "GridTile.h"
#include "FibonacciHeap.h"

enum PartDriection
{
	PD_Left_Top,
	PD_Right_Top,
	PD_Left_Bottom,
	PD_Right_Bottom
};

class TilePart
{
public:
	Vector2D vec;
	PartDriection dir;
};


class AstarResult
{
public:
	std::vector<Node> path;
	std::vector<Node> closeList;
	std::vector<Node> openList;
};

class AStar
{
	static AStar* _instance;   // The one, single instance
	AStar() {}
	AStar(const AStar&);
	AStar& operator=(const AStar&);
public:

	std::array < std::array<int, MAX_Y_TILE>, MAX_X_TILE> map;
	std::array<std::array<GridTile*, MAX_Y_TILE>, MAX_X_TILE> mapGrid;

	std::vector<Vector2D> listPosValids;

public:
	static AStar* getInstance();

	void Init();
	void SetValue(int x, int y, int value);
	AstarResult findPath(Node player, Node dest);


	bool RamdomValidPosition(Vector2D *pos); 
	bool RandomValidPosAround(Vector2D pos0, Vector2D *pos, int range); 
	//get list node valid 
	std::vector<Vector2D> GetListVecInAxisValid(int x, int y, int xRound, int yRound, int counBrick);
	Vector2D* RandomVecInAxisValid(Vector2D vecMine, Vector2D vecOther, int range);
public:
	bool isValid(int x, int y);
	bool isObstacle(int x, int y, std::array < std::array<int, MAX_Y_TILE>, MAX_X_TILE> map);
	bool isDestination(int x, int y, Node dest);
	double calculateH(int x, int y, Node dest);

	bool isValidAndNotObs(int x, int y);

	std::vector<Node> makePath(std::array<std::array<Node, MAX_Y_TILE>, MAX_X_TILE> map, Node dest);

public:
	//Part
	std::array < std::array<int, MAX_Y_TILE / PART_SIZE>, MAX_X_TILE / PART_SIZE> mapPart;
	AstarResult findPathV2(Node player, Node dest);
	AstarResult findPathV2WithPart(Node begin, Node dest, Vector2D partPlayer, Vector2D partDest, int size_part);
	std::vector<Node> findPathNodeV2(Node player, Node dest, int size_part);
	TilePart GetNextPart(Vector2D partPlayer, Vector2D partDest);
};


