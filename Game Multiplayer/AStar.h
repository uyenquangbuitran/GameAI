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


/*
Smooth path.
Apply to game.
*/


static std::set<int, std::greater <int>> obstaclesNode;

class AStar
{
public:
	static void AddObstacle(int x, int y)
	{
		int id = x + y * (WIDTH / X_STEP);
		obstaclesNode.insert(id);
	}

	static void AddObstacle(int id)
	{
		obstaclesNode.insert(id);
	}

	static bool IsValid(int x, int y)
	{
		int id = x + y * (WIDTH / X_STEP);

		if (!obstaclesNode.count(id))
		{
			if (x < 0 || y < 0 || x >= (WIDTH / X_STEP) || y >= (HEIGHT / Y_STEP))
				return false;

			return true;
		}
		return false;
	}

	static bool IsDestination(int x, int y, Node dest)
	{
		if (x == dest.x && y == dest.y)
			return true;
		return false;
	}

	static double CalculateH(int x, int y, Node dest)
	{
		return (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y)));
	}

	static std::vector<Node> MakePath(std::array<std::array<Node, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> map, Node dest)
	{
		try
		{
			std::cout << "Found a path!" << std::endl;
			int x = dest.x;
			int y = dest.y;
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
			std::cout << e.what() << std::endl;
		}
	}

	static std::vector<Node> aStar(Node begin, Node dest,
		std::array<std::array<GridTile*, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> map)
	{
		std::vector<Node> empty;
		if (!IsValid(dest.x, dest.y))
		{
			std::cout << "Invalid destination." << std::endl;
			return empty;
		}

		if (IsDestination(begin.x, begin.y, dest))
		{
			std::cout << "Destination reached!" << std::endl;
			return empty;
		}

		bool closedList[(WIDTH / X_STEP)][(HEIGHT / Y_STEP)];

		//Initalize map.
		std::array<std::array<Node, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> allMap;

		for (int x = 0; x < (WIDTH / X_STEP); x++)
		{
			for (int y = 0; y < (HEIGHT / Y_STEP); y++)
			{
				allMap[x][y].fCost = FLT_MAX;
				allMap[x][y].gCost = FLT_MAX;
				allMap[x][y].hCost = FLT_MAX;
				allMap[x][y].parentX = -1;
				allMap[x][y].parentY = -1;
				allMap[x][y].x = x;
				allMap[x][y].y = y;

				closedList[x][y] = false;

				//Change value of obstacles node.
				/*if (obstaclesNode.count(x + y * (WIDTH / X_STEP)))
					map[x][y]->SetType(Obstacle);*/
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

		map[dest.x][dest.y]->SetType(Destination);

		FibonacciHeap<Node> openList;
		openList.insert(allMap[x][y]);
		bool destinationFound = false;

		while (!openList.isEmpty() && openList.size() < (WIDTH / X_STEP) * (HEIGHT / Y_STEP))
		{
			Node node;
			do
			{
				node = openList.removeMinimum();
			} while (!IsValid(node.x, node.y));

			x = node.x;
			y = node.y;
			closedList[x][y] = true;
			map[x][y]->SetType(Closed);

			for (int newX = -1; newX <= 1; newX++)
			{
				for (int newY = -1; newY <= 1; newY++)
				{
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
								gNew = node.gCost + 1.0;
							else
								//gNew = node.gCost + 1.5;
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
								map[x + newX][y + newY]->SetType(Opened);
							}
						}
					}
				}
			}
			//PrintMatrix(allMap);
		}
		if (!destinationFound)
		{
			std::cout << "Destination not found" << std::endl;
			return empty;
		}
	}

	/*static void PrintMatrix(std::array<std::array<Node, (HEIGHT / Y_STEP)>, (WIDTH / X_STEP)> map)
	{
		for (int x = 0; x < (WIDTH / X_STEP); x++)
		{
			for (int y = 0; y < (HEIGHT / Y_STEP); y++)
			{
				std::cout << map[x][y].value << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "---------------------------------" << std::endl;
	}*/
};