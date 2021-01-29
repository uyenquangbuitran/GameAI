#pragma once
#include <stack>
#include <vector>
#include <array>
#include <math.h>
#include <cfloat>
#include <set>
#include <iostream>

#include "FibonacciHeap.h"
#include "GameDefine.h"
#include "GraphNode.h"
#include "MapTile.h"
#include "GameLog.h"

enum TileDirection
{
	TD_Left_Top,
	TD_Right_Top,
	TD_Left_Bottom,
	TD_Right_Bottom
};

class TilePart
{
public:
	Vector2D vec;
	TileDirection dir;
};

//
class AStar
{		
	static AStar* _instance;
	AStar() {}
	AStar(const AStar&);
	AStar& operator=(const AStar&);

public:
	std::array < std::array<int, MAP_HEIGHT / Y_STEP>, MAP_WIDTH / X_STEP> map;

	static AStar* GetInstance();
	void Init();

	bool IsValid(int x, int y)
	{	
		if (x < 0 || y < 0 || x >= MAP_WIDTH / X_STEP || y >= MAP_HEIGHT / Y_STEP)
			return false;
		return true;
	}

	bool IsObstacle(int x, int y)
	{
		if (IsValid(x, y))
			return map[x][y] != 0;
		return false;
	}

	bool IsValidAndNotObstacle(int x, int y)
	{
		return IsValid(x, y) && !IsObstacle(x, y);
	}

	bool IsDestination(int x, int y, Node dest)
	{
		if (x == dest.x && y == dest.y)
			return true;
		return false;
	}

	double CalculateH(int x, int y, Node dest)
	{
		return (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y)));
	}

	std::vector<Node> MakePath(std::array<std::array<Node, MAP_HEIGHT / Y_STEP>, MAP_WIDTH / X_STEP> map, Node dest)
	{
		try
		{
			//GAMELOG("Found a path!");
			int x = dest.x;
			int y = dest.y;
			dest.fCost = 0;
			std::stack<Node> path;
			std::vector<Node> validPath;
			while (!(map[x][y].parentX == x && map[x][y].parentY == y)
				&& map[x][y].x != -1 && map[x][y].y != -1)
			{
				path.push(map[x][y]);
				int tempX = map[x][y].parentX;
				int tempY = map[x][y].parentY;
				x = tempX;
				y = tempY;
			}
			path.push(map[x][y]);

			while (!path.empty())
			{
				Node top = path.top();
				path.pop();
				validPath.emplace_back(top);
			}

			return validPath;
		}
		catch (const std::exception& e)
		{
			GAMELOG(e.what());
		}
	}

	TilePart GetNextPart(Vector2D beginPart, Vector2D destPart)
	{
		TilePart part;
		//Begin tile as the origin.
		int dx = destPart.x - beginPart.x;
		int dy = destPart.y - beginPart.y;

		float absTan = (dx == 0) ? dy : abs(dy / dx);

		if (dx > 0)
		{
			if (dy > 0)
			{
				part.dir = TD_Left_Top;
				if (absTan <= 0.33f) //[x + 1, y]
				{
					part.vec.x = beginPart.x + 1;
					part.vec.y = beginPart.y;
				}
				else if (absTan <= 3) // [x + 1, y + 1]
				{
					part.vec.x = beginPart.x + 1;
					part.vec.y = beginPart.y + 1;
				}
				else // [x, y + 1]
				{
					part.vec.x = beginPart.x;
					part.vec.y = beginPart.y + 1;
				}
			}
			else
			{
				part.dir = TD_Left_Bottom;
				if (absTan <= 0.33f) //[x + 1, y]
				{
					part.vec.x = beginPart.x + 1;
					part.vec.y = beginPart.y;
				}
				else if (absTan <= 3) // [x + 1, y - 1]
				{
					part.vec.x = beginPart.x + 1;
					part.vec.y = beginPart.y - 1;
				}
				else // [x, y - 1]
				{
					part.vec.x = beginPart.x;
					part.vec.y = beginPart.y - 1;				
				}
			}
		}
		else
		{
			if (dy > 0)
			{
				part.dir = TD_Right_Top;
				if (absTan <= 0.33f) //[x - 1, y]
				{
					part.vec.x = beginPart.x - 1;
					part.vec.y = beginPart.y;
				}
				else if (absTan <= 3) // [x - 1, y + 1]
				{
					part.vec.x = beginPart.x - 1;
					part.vec.y = beginPart.y + 1;
				}
				else // [x, y + 1]
				{
					part.vec.x = beginPart.x;
					part.vec.y = beginPart.y + 1;
				}
			}
			else
			{
				part.dir = TD_Right_Bottom;
				if (absTan <= 0.33f) //[x - 1, y]
				{
					part.vec.x = beginPart.x - 1;
					part.vec.y = beginPart.y;
				}
				else if (absTan <= 3) // [x - 1, y - 1]
				{
					part.vec.x = beginPart.x - 1;
					part.vec.y = beginPart.y - 1;
				}
				else // [x, y - 1]
				{
					part.vec.x = beginPart.x;
					part.vec.y = beginPart.y - 1;
				}
			}
		}

		return part;
	}

	std::vector<Node> aStar(Node begin, Node dest, int partSize)
	{	
		if (abs(begin.x - dest.x) <= partSize && abs(begin.y - dest.y) <= partSize)
		{
			Vector2D beginPart, destPart;
			beginPart.x = begin.x / partSize;
			beginPart.y = begin.y / partSize;
			destPart.x = dest.x / partSize;
			destPart.x = dest.x / partSize;
			/*beginPart.x = begin.x;
			beginPart.y = begin.y;
			destPart.x = dest.x;
			destPart.x = dest.x;*/
			return aStarWithPart(begin, dest, beginPart, destPart, partSize);
		}

		Vector2D beginPart, destPart;
		beginPart.x = begin.x / partSize;
		beginPart.y = begin.y / partSize;
		destPart.x = dest.x / partSize;
		destPart.x = dest.x / partSize;

		if (beginPart.x == destPart.x && beginPart.y == destPart.y)
		{
			return aStarWithPart(begin, dest, beginPart, destPart, partSize);
		}
		else
		{
			Node next;
			TilePart nextPart = GetNextPart(beginPart, destPart);

			switch (nextPart.dir)
			{
			case TD_Left_Top:
				for (int counter = 0; counter <= partSize; counter++)
				{
					for (int x = nextPart.vec.x * partSize; x <= nextPart.vec.x * partSize + counter; x++)
					{
						for (int y = nextPart.vec.y * partSize + counter; y >= nextPart.vec.y * partSize; y--)
						{
							if (IsValidAndNotObstacle(x, y))
							{
								next.x = x;
								next.y = y;
								return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
							}
						}
					}
				}

			case TD_Left_Bottom:
				for (int counter = 0; counter <= partSize; counter++)
				{
					for (int x = nextPart.vec.x * partSize + counter; x >= nextPart.vec.x * partSize; x--)
					{
						for (int y = nextPart.vec.y * partSize; y <= nextPart.vec.y * partSize + counter; y++)
						{
							if (IsValidAndNotObstacle(x, y))
							{
								next.x = x;
								next.y = y;
								return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
							}
						}
					}
				}
				
			case TD_Right_Top:
				for (int counter = 0; counter <= partSize; counter++)
				{
					for (int x = nextPart.vec.x * partSize - counter; x <= nextPart.vec.x * partSize; x++)
					{
						for (int y = nextPart.vec.y * partSize; y <= nextPart.vec.y * partSize + counter; y++)
						{
							if (IsValidAndNotObstacle(x, y))
							{
								next.x = x;
								next.y = y;
								return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
							}
						}
					}
				}

			case TD_Right_Bottom:
				for (int counter = 0; counter <= partSize; counter++)
				{
					for (int x = nextPart.vec.x * partSize; x >= nextPart.vec.x * partSize - counter; x--)
					{
						for (int y = nextPart.vec.y * partSize - counter; y <= nextPart.vec.y * partSize + counter; y++)
						{
							if (IsValidAndNotObstacle(x, y))
							{
								next.x = x;
								next.y = y;
								return aStarWithPart(begin, next, beginPart, nextPart.vec, partSize);
							}
						}
					}
				}
			default:
				break;
			}

			return aStar(begin, dest, partSize + PART_SIZE);
		}		
	}

	std::vector<Node> aStarWithPart(Node begin, Node dest, Vector2D beginPart, Vector2D destPart, int partSize)
	{
		std::vector<Node> empty;
		if (!IsValid(dest.x, dest.y))
		{
			GAMELOG("Invalid destination!");
			return empty;
		}

		if (IsObstacle(dest.x, dest.y))
		{
			GAMELOG("Destination is an obstacle!");
			return empty;
		}

		if (IsDestination(begin.x, begin.y, dest))
		{
			GAMELOG("Destination reached!");
			return empty;
		}

		int xMin, xMax, yMin, yMax;

		if (beginPart.x < destPart.x) {
			xMin = beginPart.x * partSize;
			xMax = (destPart.x + 1)*partSize;
		}
		else
		{
			xMin = destPart.x * partSize;
			xMax = (beginPart.x + 1) * partSize;
		}

		if (beginPart.y < destPart.y) {
			yMin = beginPart.y * partSize;
			yMax = (destPart.y + 1) * partSize;
		}
		else
		{
			yMin = destPart.y * partSize;
			yMax = (beginPart.y + 1) * partSize;
		}

		if (xMax > MAP_WIDTH / X_STEP) xMax = MAP_WIDTH / X_STEP;
		if (yMax > MAP_HEIGHT / Y_STEP) yMax = MAP_HEIGHT / Y_STEP;

		//Initalize map.
		std::array<std::array<Node, MAP_HEIGHT / Y_STEP>, MAP_WIDTH / X_STEP> allMap;
		bool closedList[MAP_HEIGHT / Y_STEP][MAP_WIDTH / X_STEP];		

		while (partSize < MAP_HEIGHT / Y_STEP / 2)
		{
			
			for (int x = xMin; x < xMax; x++) {
				for (int y = yMin; y < yMax; y++) {
					allMap[x][y].fCost = FLT_MAX;
					allMap[x][y].gCost = FLT_MAX;
					allMap[x][y].hCost = FLT_MAX;
					allMap[x][y].parentX = -1;
					allMap[x][y].parentY = -1;
					allMap[x][y].x = x;
					allMap[x][y].y = y;

					closedList[x][y] = false;
				}
			}

			//Initialize starting list.
			int x = begin.x;
			int y = begin.y;
			allMap[x][y].fCost = 0.0;
			allMap[x][y].gCost = 0.0;
			allMap[x][y].hCost = 0.0;
			allMap[x][y].parentX = x;
			allMap[x][y].parentY = y;			
			bool destinationFound = false;

			FibonacciHeap<Node> openList;
			openList.insert(allMap[x][y]);

			while (!openList.isEmpty() && openList.size() < (MAP_WIDTH / X_STEP) * (MAP_HEIGHT / Y_STEP))
			{
				Node node;
				do
					node = openList.removeMinimum();
				while (!openList.isEmpty() && !IsValidAndNotObstacle(node.x, node.y));

				x = node.x;
				y = node.y;
				closedList[x][y] = true;

				for (int newX = -1; newX <= 1; newX++)
				{
					for (int newY = -1; newY <= 1; newY++)
					{
						if (x + newX < xMin || x + newX > xMax
							|| y + newY < yMin || y + newY > yMax) continue;

						double gNew, hNew, fNew;
						if (IsValid(x + newX, y + newY))
						{
							if (IsDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0))
							{
								//Destination found - make path.
								allMap[x + newX][y + newY].parentX = x;
								allMap[x + newX][y + newY].parentY = y;
								destinationFound = true;								
								return MakePath(allMap, dest);
							}
							else if (!closedList[x + newX][y + newY])
							{
								if (newX == 0 || newY == 0)
									gNew = node.gCost + 4.5f;
								else									
									continue;
								hNew = CalculateH(x + newX, y + newY, dest);
								fNew = gNew + hNew;

								//Check if this path is better than the one already present.
								if (allMap[x + newX][y + newY].fCost == FLT_MAX || allMap[x + newX][y + newY].fCost > fNew)
								{
									allMap[x + newX][y + newY].fCost = fNew;
									allMap[x + newX][y + newY].gCost = gNew;
									allMap[x + newX][y + newY].hCost = hNew;
									allMap[x + newX][y + newY].parentX = x;
									allMap[x + newX][y + newY].parentY = y;
									openList.insert(allMap[x + newX][y + newY]);
								}
							}
						}
					}
				}
			}

		}
	}

	void SetTileValue(int x, int y, int value)
	{
		if (IsValid(x, y)) map[x][y] = value;
	}

#pragma region OldAStar
	
	//static std::vector<Node> MakePathLv1(std::array<std::array<Node, LEVEL1_HEIGHT>, LEVEL1_WIDTH> map, Node dest, int startValueX = 0, int startValueY = 0)
	//{
	//	try
	//	{
	//		//GAMELOG("Found a path!");
	//		int x = dest.x - startValueX;
	//		int y = dest.y - startValueY;
	//		dest.fCost = 0;
	//		std::stack<Node> path;
	//		std::vector<Node> validPath;
	//		while (!(map[x][y].parentX == x
	//			&& map[x][y].parentY == y)
	//			&& map[x][y].x != -1
	//			&& map[x][y].y != -1)
	//		{
	//			path.push(map[x][y]);
	//			int tempX = map[x][y].parentX;
	//			int tempY = map[x][y].parentY;
	//			x = tempX;
	//			y = tempY;
	//		}
	//		path.push(map[x][y]);

	//		while (!path.empty())
	//		{
	//			Node top = path.top();
	//			path.pop();
	//			top.x += startValueX;
	//			top.y += startValueY;
	//			validPath.emplace_back(top);
	//		}

	//		return validPath;
	//	}
	//	catch (const std::exception& e)
	//	{
	//		GAMELOG(e.what());
	//	}
	//}

	//static std::vector<Node> MakePathLv2(std::array<std::array<Node, LEVEL2_HEIGHT>, LEVEL2_HEIGHT> map, Node dest, int startValueX = 0, int startValueY = 0)
	//{
	//	try
	//	{
	//		//GAMELOG("Found a path!");
	//		int x = dest.x - startValueX;
	//		int y = dest.y - startValueY;
	//		dest.fCost = 0;
	//		std::stack<Node> path;
	//		std::vector<Node> validPath;
	//		while (!(map[x][y].parentX == x
	//			&& map[x][y].parentY == y)
	//			&& map[x][y].x != -1
	//			&& map[x][y].y != -1)
	//		{
	//			path.push(map[x][y]);
	//			int tempX = map[x][y].parentX;
	//			int tempY = map[x][y].parentY;
	//			x = tempX;
	//			y = tempY;
	//		}
	//		path.push(map[x][y]);

	//		while (!path.empty())
	//		{
	//			Node top = path.top();
	//			path.pop();
	//			top.x += startValueX;
	//			top.y += startValueY;
	//			validPath.emplace_back(top);
	//		}

	//		return validPath;
	//	}
	//	catch (const std::exception& e)
	//	{
	//		GAMELOG(e.what());
	//	}
	//}

	//static std::vector<Node> MakePathLv3(std::array<std::array<Node, LEVEL3_HEIGHT>, LEVEL3_WIDTH> map, Node dest, int startValueX = 0, int startValueY = 0)
	//{
	//	try
	//	{
	//		//GAMELOG("Found a path!");
	//		int x = dest.x - startValueX;
	//		int y = dest.y - startValueY;
	//		dest.fCost = 0;
	//		std::stack<Node> path;
	//		std::vector<Node> validPath;
	//		while (!(map[x][y].parentX == x
	//			&& map[x][y].parentY == y)
	//			&& map[x][y].x != -1
	//			&& map[x][y].y != -1)
	//		{
	//			path.push(map[x][y]);
	//			int tempX = map[x][y].parentX;
	//			int tempY = map[x][y].parentY;
	//			x = tempX;
	//			y = tempY;
	//		}
	//		path.push(map[x][y]);

	//		while (!path.empty())
	//		{
	//			Node top = path.top();
	//			path.pop();
	//			top.x += startValueX;
	//			top.y += startValueY;
	//			validPath.emplace_back(top);
	//		}

	//		return validPath;
	//	}
	//	catch (const std::exception& e)
	//	{
	//		GAMELOG(e.what());
	//	}
	//}

	//static std::vector<Node> aStarLv1(Node begin, Node dest,
	//	std::vector<MapTile*> tileList,
	//	std::array<std::array<MapTileLv2*, LEVEL1_HEIGHT>, LEVEL1_WIDTH> map,
	//	std::set<int, std::greater <int>> obstaclesNode)
	//{
	//	std::vector<Node> empty;
	//	if (!IsValid(dest.x, dest.y, obstaclesNode, MAP_WIDTH, MAP_HEIGHT/*, tankNodes*/))
	//	{
	//	//GAMELOG("Invalid destination.");
	//	return empty;
	//	}

	//if (IsDestination(begin.x, begin.y, dest))
	//{
	//	//GAMELOG("Destination reached!");
	//	return empty;
	//}

	//std::vector<std::vector<bool>> closedList;
	//closedList.resize(LEVEL1_HEIGHT);
	//for (int counter = 0; counter < LEVEL1_HEIGHT; counter++)
	//{
	//	closedList[counter].resize(LEVEL1_HEIGHT);
	//}

	//int startValueX = map[0][0]->GetX();
	//int startValueY = map[0][0]->GetY();

	////Initalize map.
	//std::array<std::array<Node, (LEVEL1_HEIGHT)>, (LEVEL1_WIDTH)> allMap;
	//for (int x = 0; x < LEVEL1_HEIGHT; x++)
	//{
	//	for (int y = 0; y < LEVEL1_WIDTH; y++)
	//	{
	//		allMap[x][y].fCost = FLT_MAX;
	//		allMap[x][y].gCost = FLT_MAX;
	//		allMap[x][y].hCost = FLT_MAX;
	//		allMap[x][y].parentX = -1;
	//		allMap[x][y].parentY = -1;
	//		allMap[x][y].x = map[x][y]->GetX() - startValueX;
	//		allMap[x][y].y = map[x][y]->GetY() - startValueY;
	//		closedList[x][y] = false;
	//	}
	//}

	//Node tempDes; tempDes.x = dest.x - startValueX; tempDes.y = dest.y - startValueY;

	////Initialize starting list.
	//int x = begin.x - startValueX;
	//int y = begin.y - startValueY;
	//allMap[x][y].fCost = 0.0;
	//allMap[x][y].gCost = 0.0;
	//allMap[x][y].hCost = 0.0;
	//allMap[x][y].parentX = x;
	//allMap[x][y].parentY = y;
	//allMap[x][y].status = 1;

	//FibonacciHeap<Node> openList;

	//openList.insert(allMap[x][y]);
	//bool destinationFound = false;

	//while (!openList.isEmpty() && openList.size() < LEVEL1_HEIGHT * LEVEL1_WIDTH)
	//{
	//	Node node;
	//	do
	//	{
	//		node = openList.removeMinimum();
	//	} while (!IsValid(node.x, node.y, obstaclesNode, LEVEL1_WIDTH, LEVEL1_HEIGHT/*, tankNodes*/, startValueX, startValueY));
	//	/*node.status = -1;*/
	//	x = node.x;
	//	y = node.y;
	//	closedList[x][y] = true;

	//	for (int newX = -1; newX <= 1; newX++)
	//	{
	//		for (int newY = -1; newY <= 1; newY++)
	//		{
	//			double gNew, hNew, fNew;
	//			if (IsValid(x + newX, y + newY, obstaclesNode, LEVEL1_WIDTH, LEVEL1_HEIGHT/*, tankNodes*/, startValueX, startValueY))
	//			{

	//				if (IsDestination(x + newX, y + newY, dest, startValueX, startValueY) && (newX == 0 || newY == 0))
	//				{
	//					//Destination found - make path.

	//					gNew = node.gCost + map[x + newX][y + newY]->GetCost();
	//					hNew = CalculateH(x + newX, y + newY, tempDes);
	//					fNew = gNew + hNew;
	//					allMap[x + newX][y + newY].fCost = fNew;
	//					allMap[x + newX][y + newY].gCost = gNew;
	//					allMap[x + newX][y + newY].hCost = hNew;
	//					allMap[x + newX][y + newY].parentX = x;
	//					allMap[x + newX][y + newY].parentY = y;
	//					destinationFound = true;
	//					return MakePathLv1(allMap, dest, startValueX, startValueY);
	//				}
	//				else if (!closedList[x + newX][y + newY])
	//				{
	//					if ((newX == 0 || newY == 0) && map[x + newX][y + newY]->GetCost() != -1)
	//						gNew = node.gCost + map[x + newX][y + newY]->GetCost();
	//					else continue;

	//					hNew = CalculateH(x + newX, y + newY, tempDes);
	//					fNew = gNew + hNew;

	//					//Check if this path is better than the one already present.
	//					if (allMap[x + newX][y + newY].fCost == FLT_MAX
	//						|| allMap[x + newX][y + newY].fCost > fNew)
	//					{
	//						allMap[x + newX][y + newY].fCost = fNew;
	//						allMap[x + newX][y + newY].gCost = gNew;
	//						allMap[x + newX][y + newY].hCost = hNew;
	//						allMap[x + newX][y + newY].parentX = x;
	//						allMap[x + newX][y + newY].parentY = y;
	//						/*allMap[x + newX][y + newY].status = 1;*/
	//						openList.insert(allMap[x + newX][y + newY]);
	//						/*for (int i = 0; i < 15; i++)
	//						{
	//							GAMELOG("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", allMap[0][i].status, allMap[1][i].status, allMap[2][i].status, allMap[3][i].status,
	//								allMap[4][i].status, allMap[5][i].status, allMap[6][i].status, allMap[7][i].status, allMap[8][i].status, allMap[9][i].status,
	//								allMap[10][i].status, allMap[11][i].status, allMap[12][i].status, allMap[13][i].status, allMap[14][i].status);
	//						}
	//						GAMELOG("----------");*/
	//					}
	//				}
	//			}

	//		}
	//	}
	//}
	//if (!destinationFound)
	//{
	//	GAMELOG("Destination not found");
	//	return empty;
	//}
	//}

	//static std::vector<Node> aStarLv2(Node begin, Node dest,
	//	std::vector<MapTile*> tileList,
	//	std::array<std::array<MapTileLv2*, LEVEL2_HEIGHT>, LEVEL2_WIDTH> map,
	//	std::set<int, std::greater <int>> obstaclesNode)
	//{
	//	std::vector<Node> empty;
	//	if (!IsValid(dest.x, dest.y, obstaclesNode, MAP_WIDTH, MAP_HEIGHT/*, tankNodes*/))
	//	{
	//		//GAMELOG("Invalid destination.");
	//		return empty;
	//	}

	//	if (IsDestination(begin.x, begin.y, dest))
	//	{
	//		//GAMELOG("Destination reached!");
	//		return empty;
	//	}

	//	std::vector<std::vector<bool>> closedList;
	//	closedList.resize(LEVEL2_HEIGHT);
	//	for (int counter = 0; counter < LEVEL2_HEIGHT; counter++)
	//	{
	//		closedList[counter].resize(LEVEL2_HEIGHT);
	//	}

	//	int startValueX = map[0][0]->GetX();
	//	int startValueY = map[0][0]->GetY();

	//	//Initalize map.
	//	std::array<std::array<Node, (LEVEL2_HEIGHT)>, (LEVEL2_WIDTH)> allMap;
	//	for (int x = 0; x < LEVEL2_HEIGHT; x++)
	//	{
	//		for (int y = 0; y < LEVEL2_WIDTH; y++)
	//		{
	//			allMap[x][y].fCost = FLT_MAX;
	//			allMap[x][y].gCost = FLT_MAX;
	//			allMap[x][y].hCost = FLT_MAX;
	//			allMap[x][y].parentX = -1;
	//			allMap[x][y].parentY = -1;
	//			allMap[x][y].x = map[x][y]->GetX() - startValueX;
	//			allMap[x][y].y = map[x][y]->GetY() - startValueY;
	//			closedList[x][y] = false;
	//		}
	//	}

	//	Node tempDes; tempDes.x = dest.x - startValueX; tempDes.y = dest.y - startValueY;

	//	//Initialize starting list.
	//	int x = begin.x - startValueX;
	//	int y = begin.y - startValueY;
	//	allMap[x][y].fCost = 0.0;
	//	allMap[x][y].gCost = 0.0;
	//	allMap[x][y].hCost = 0.0;
	//	allMap[x][y].parentX = x;
	//	allMap[x][y].parentY = y;
	//	allMap[x][y].status = 1;

	//	FibonacciHeap<Node> openList;

	//	openList.insert(allMap[x][y]);
	//	bool destinationFound = false;

	//	while (!openList.isEmpty() && openList.size() < LEVEL2_HEIGHT * LEVEL2_WIDTH)
	//	{
	//		Node node;
	//		do
	//		{
	//			node = openList.removeMinimum();
	//		} while (!IsValid(node.x, node.y, obstaclesNode, LEVEL2_WIDTH, LEVEL2_HEIGHT/*, tankNodes*/, startValueX, startValueY));
	//		/*node.status = -1;*/
	//		x = node.x;
	//		y = node.y;
	//		closedList[x][y] = true;

	//		for (int newX = -1; newX <= 1; newX++)
	//		{
	//			for (int newY = -1; newY <= 1; newY++)
	//			{
	//				double gNew, hNew, fNew;
	//				if (IsValid(x + newX, y + newY, obstaclesNode, LEVEL2_WIDTH, LEVEL2_HEIGHT/*, tankNodes*/, startValueX, startValueY))
	//				{

	//					if (IsDestination(x + newX, y + newY, dest, startValueX, startValueY) && (newX == 0 || newY == 0))
	//					{
	//						//Destination found - make path.

	//						gNew = node.gCost + map[x + newX][y + newY]->GetCost();
	//						hNew = CalculateH(x + newX, y + newY, tempDes);
	//						fNew = gNew + hNew;
	//						allMap[x + newX][y + newY].fCost = fNew;
	//						allMap[x + newX][y + newY].gCost = gNew;
	//						allMap[x + newX][y + newY].hCost = hNew;
	//						allMap[x + newX][y + newY].parentX = x;
	//						allMap[x + newX][y + newY].parentY = y;
	//						destinationFound = true;
	//						return MakePathLv2(allMap, dest, startValueX, startValueY);
	//					}
	//					else if (!closedList[x + newX][y + newY])
	//					{
	//						if ((newX == 0 || newY == 0) && map[x + newX][y + newY]->GetCost() != -1)
	//							gNew = node.gCost + map[x + newX][y + newY]->GetCost();
	//						else continue;

	//						hNew = CalculateH(x + newX, y + newY, tempDes);
	//						fNew = gNew + hNew;

	//						//Check if this path is better than the one already present.
	//						if (allMap[x + newX][y + newY].fCost == FLT_MAX
	//							|| allMap[x + newX][y + newY].fCost > fNew)
	//						{
	//							allMap[x + newX][y + newY].fCost = fNew;
	//							allMap[x + newX][y + newY].gCost = gNew;
	//							allMap[x + newX][y + newY].hCost = hNew;
	//							allMap[x + newX][y + newY].parentX = x;
	//							allMap[x + newX][y + newY].parentY = y;
	//							/*allMap[x + newX][y + newY].status = 1;*/
	//							openList.insert(allMap[x + newX][y + newY]);
	//							/*for (int i = 0; i < 15; i++)
	//							{
	//								GAMELOG("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", allMap[0][i].status, allMap[1][i].status, allMap[2][i].status, allMap[3][i].status,
	//									allMap[4][i].status, allMap[5][i].status, allMap[6][i].status, allMap[7][i].status, allMap[8][i].status, allMap[9][i].status,
	//									allMap[10][i].status, allMap[11][i].status, allMap[12][i].status, allMap[13][i].status, allMap[14][i].status);
	//							}
	//							GAMELOG("----------");*/
	//						}
	//					}
	//				}

	//			}
	//		}
	//	}
	//	if (!destinationFound)
	//	{
	//		GAMELOG("Destination not found");
	//		return empty;
	//	}
	//}

	//static std::vector<Node> aStarLv3(Node begin, Node dest,
	//	std::array<std::array<MapTileLv3*, LEVEL3_HEIGHT>, LEVEL3_WIDTH> map,
	//	std::set<int, std::greater <int>> obstaclesNode/*,
	//	std::set<int, std::greater <int>> tankNodes*/)
	//{
	//	std::vector<Node> empty;
	//	if (!IsValid(dest.x, dest.y, obstaclesNode, MAP_WIDTH, MAP_HEIGHT/*, tankNodes*/))
	//	{
	//		//GAMELOG("Invalid destination.");
	//		return empty;
	//	}

	//	if (IsDestination(begin.x, begin.y, dest))
	//	{
	//		//GAMELOG("Destination reached!");
	//		return empty;
	//	}

	//	std::vector<std::vector<bool>> closedList;
	//	closedList.resize(LEVEL3_HEIGHT);
	//	for (int counter = 0; counter < LEVEL3_HEIGHT; counter++)
	//	{
	//		closedList[counter].resize(LEVEL3_WIDTH);
	//	}

	//	int startValueX = map[0][0]->GetX();
	//	int startValueY = map[0][0]->GetY();

	//	//Initalize map.
	//	std::array<std::array<Node, (LEVEL3_HEIGHT)>, (LEVEL3_WIDTH)> allMap;
	//	for (int x = 0; x < LEVEL3_HEIGHT; x++)
	//	{
	//		for (int y = 0; y < LEVEL3_WIDTH; y++)
	//		{
	//			allMap[x][y].fCost = FLT_MAX;
	//			allMap[x][y].gCost = FLT_MAX;
	//			allMap[x][y].hCost = FLT_MAX;
	//			allMap[x][y].parentX = -1;
	//			allMap[x][y].parentY = -1;
	//			allMap[x][y].x = map[x][y]->GetX() - startValueX;
	//			allMap[x][y].y = map[x][y]->GetY() - startValueY;
	//			closedList[x][y] = false;
	//		}
	//	}

	//	Node tempDes; tempDes.x = dest.x - startValueX; tempDes.y = dest.y - startValueY;

	//	//Initialize starting list.
	//	int x = begin.x - startValueX;
	//	int y = begin.y - startValueY;
	//	allMap[x][y].fCost = 0.0;
	//	allMap[x][y].gCost = 0.0;
	//	allMap[x][y].hCost = 0.0;
	//	allMap[x][y].parentX = x;
	//	allMap[x][y].parentY = y;
	//	allMap[x][y].status = 1;

	//	FibonacciHeap<Node> openList;

	//	openList.insert(allMap[x][y]);
	//	bool destinationFound = false;

	//	while (!openList.isEmpty() && openList.size() < LEVEL3_WIDTH * LEVEL3_HEIGHT)
	//	{
	//		Node node;
	//		do
	//		{
	//			node = openList.removeMinimum();
	//		} while (!IsValid(node.x, node.y, obstaclesNode, LEVEL3_WIDTH, LEVEL3_HEIGHT/*, tankNodes*/, startValueX, startValueY));
	//		/*node.status = -1;*/
	//		x = node.x;
	//		y = node.y;
	//		closedList[x][y] = true;

	//		for (int newX = -1; newX <= 1; newX++)
	//		{
	//			for (int newY = -1; newY <= 1; newY++)
	//			{
	//				double gNew, hNew, fNew;
	//				if (IsValid(x + newX, y + newY, obstaclesNode, LEVEL3_WIDTH, LEVEL3_HEIGHT/*, tankNodes*/, startValueX, startValueY))
	//				{

	//					if (IsDestination(x + newX, y + newY, dest, startValueX, startValueY) && (newX == 0 || newY == 0))
	//					{
	//						//Destination found - make path.

	//						gNew = node.gCost + map[x + newX][y + newY]->GetCost();
	//						hNew = CalculateH(x + newX, y + newY, tempDes);
	//						fNew = gNew + hNew;
	//						allMap[x + newX][y + newY].fCost = fNew;
	//						allMap[x + newX][y + newY].gCost = gNew;
	//						allMap[x + newX][y + newY].hCost = hNew;
	//						allMap[x + newX][y + newY].parentX = x;
	//						allMap[x + newX][y + newY].parentY = y;
	//						destinationFound = true;
	//						return MakePathLv3(allMap, dest, startValueX, startValueY);
	//					}
	//					else if (!closedList[x + newX][y + newY])
	//					{
	//						if ((newX == 0 || newY == 0) && map[x + newX][y + newY]->GetCost() != -1)
	//							gNew = node.gCost + map[x + newX][y + newY]->GetCost();
	//						else continue;

	//						hNew = CalculateH(x + newX, y + newY, tempDes);
	//						fNew = gNew + hNew;

	//						//Check if this path is better than the one already present.
	//						if (allMap[x + newX][y + newY].fCost == FLT_MAX
	//							|| allMap[x + newX][y + newY].fCost > fNew)
	//						{
	//							allMap[x + newX][y + newY].fCost = fNew;
	//							allMap[x + newX][y + newY].gCost = gNew;
	//							allMap[x + newX][y + newY].hCost = hNew;
	//							allMap[x + newX][y + newY].parentX = x;
	//							allMap[x + newX][y + newY].parentY = y;
	//							/*allMap[x + newX][y + newY].status = 1;*/
	//							openList.insert(allMap[x + newX][y + newY]);
	//							/*for (int i = 0; i < 15; i++)
	//							{
	//								GAMELOG("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", allMap[0][i].status, allMap[1][i].status, allMap[2][i].status, allMap[3][i].status,
	//									allMap[4][i].status, allMap[5][i].status, allMap[6][i].status, allMap[7][i].status, allMap[8][i].status, allMap[9][i].status,
	//									allMap[10][i].status, allMap[11][i].status, allMap[12][i].status, allMap[13][i].status, allMap[14][i].status);
	//							}
	//							GAMELOG("----------");*/
	//						}
	//					}
	//				}

	//			}
	//		}
	//	}
	//	if (!destinationFound)
	//	{
	//		GAMELOG("Destination not found");
	//		return empty;
	//	}
	//}
#pragma endregion

	
};