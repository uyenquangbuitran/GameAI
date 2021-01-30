#pragma once
#include"Astar.h";      /* time */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */

// Define the static Singleton pointer
AStar* AStar::_instance = NULL;

AStar* AStar::getInstance() {
	if (_instance == NULL) {
		_instance = new AStar();
		_instance->Init();
	}
	return(_instance);
}

void AStar::Init()
{
	for (int x = 0; x < MAX_X_TILE; x++)
	{
		for (int y = 0; y < MAX_Y_TILE; y++)
		{
			mapGrid[x][y] = new GridTile();
			mapGrid[x][y]->SetVec(x, y);
			mapGrid[x][y]->SetPosition(D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP));
			map[x][y] = 0;
		}
	}
}

bool AStar::isValid(int x, int y)
{ //If is not valid
	if (x < 0 || y < 0 || x >= MAX_X_TILE || y >= MAX_Y_TILE) {
		return false;
	}
	return true;
}

bool AStar::isObstacle(int x, int y, std::array < std::array<int, MAX_Y_TILE>, MAX_X_TILE> map)
{
	if (isValid(x, y)) {
		return map[x][y] != 0;
	}
	return false;
}

bool AStar::isValidAndNotObs(int x, int y)
{
	return isValid(x, y) && !isObstacle(x, y, map);
}

bool AStar::isDestination(int x, int y, Node dest) {
	if (x == dest.x && y == dest.y) {
		return true;
	}
	return false;
}

double AStar::calculateH(int x, int y, Node dest) {
	double H = (sqrt((x - dest.x)*(x - dest.x)
		+ (y - dest.y))*(y - dest.y));
	return H;
}

void AStar::SetValue(int x, int y, int value)
{
	if (isValid(x, y))
	{
		map[x][y] = value;
	}
}

std::vector<Node> AStar::makePath(std::array<std::array<Node, MAX_Y_TILE>, MAX_X_TILE> map, Node dest)
{
	try
	{
		GAMELOG("Found a path!");
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

AstarResult AStar::findPath(Node begin, Node dest)
{
	AstarResult result;

	if (!isValid(dest.x, dest.y))
	{
		GAMELOG("Destination is not valid");
		return result;
	}

	if (isObstacle(dest.x, dest.y, map))
	{
		GAMELOG("Destination is an obstacle");
		return result;
	}

	if (isDestination(begin.x, begin.y, dest)) {
		GAMELOG("You are the destination");
		return result;
	}

	bool closedList[MAX_X_TILE][MAX_Y_TILE];

	//Initialize whole map
	std::array<std::array < Node, MAX_Y_TILE>, MAX_X_TILE> _allMap;
	for (int x = 0; x < MAX_X_TILE; x++) {
		for (int y = 0; y < MAX_Y_TILE; y++) {
			_allMap[x][y].fCost = FLT_MAX;
			_allMap[x][y].gCost = FLT_MAX;
			_allMap[x][y].hCost = FLT_MAX;
			_allMap[x][y].parentX = -1;
			_allMap[x][y].parentY = -1;
			_allMap[x][y].x = x;
			_allMap[x][y].y = y;

			closedList[x][y] = false;
		}
	}

	//Initialize our starting list
	int x = begin.x;
	int y = begin.y;
	_allMap[x][y].fCost = 0.0;
	_allMap[x][y].gCost = 0.0;
	_allMap[x][y].hCost = 0.0;
	_allMap[x][y].parentX = x;
	_allMap[x][y].parentY = y;


	result.openList.emplace_back(_allMap[x][y]);
	bool destinationFound = false;
	FibonacciHeap<Node> openList;

	while (!openList.isEmpty() && openList.size() < MAX_X_TILE * MAX_Y_TILE)
	{
		Node node;
		do
		{
			node = openList.removeMinimum();
		} while (!openList.isEmpty() && !isValidAndNotObs(node.x, node.y));


		x = node.x;
		y = node.y;
		closedList[x][y] = true;		

		for (int newX = -1; newX <= 1; newX++)
		{
			for (int newY = -1; newY <= 1; newY++)
			{
				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY))
				{
					if (isDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0))
					{
						//Destination found - make path.
						_allMap[x + newX][y + newY].parentX = x;
						_allMap[x + newX][y + newY].parentY = y;
						destinationFound = true;
						result.path = makePath(_allMap, dest);
						return result;
					}
					else if (!closedList[x + newX][y + newY])
					{
						if (newX == 0 || newY == 0)
							gNew = node.gCost + 2.5f;
						else
							continue;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;

						//Check if this path is better than the one already present.
						if (_allMap[x + newX][y + newY].fCost == FLT_MAX || _allMap[x + newX][y + newY].fCost > fNew)
						{
							_allMap[x + newX][y + newY].fCost = fNew;
							_allMap[x + newX][y + newY].gCost = gNew;
							_allMap[x + newX][y + newY].hCost = hNew;
							_allMap[x + newX][y + newY].parentX = x;
							_allMap[x + newX][y + newY].parentY = y;
							openList.insert(_allMap[x + newX][y + newY]);
						}
					}
				}
			}
		}		
	}
	if (destinationFound == false) {
		GAMELOG("Destination not found");
		return result;
	}

	return result;
}


bool AStar::RamdomValidPosition(Vector2D *pos)
{
	int partX = rand() % (NUM_PART_X);
	int partY = rand() % (NUM_PART_Y);

	Vector2D posPart;
	posPart.x = (partX + 0.5f) * PART_SIZE;
	posPart.y = (partY + 0.5f) * PART_SIZE;

	return RandomValidPosAround(posPart, pos, PART_SIZE);
}

bool AStar::RandomValidPosAround(Vector2D pos0, Vector2D *pos, int range)
{
	std::vector<Vector2D> validPosList;

	int xMin, xMax, yMin, yMax;
	xMin = pos0.x - range;
	xMax = pos0.x + range;
	yMin = pos0.y - range;
	yMax = pos0.y + range;
	xMin = xMin < 0 ? 0 : xMin;
	xMax = xMax > MAX_X_TILE ? MAX_X_TILE : xMax;
	yMin = yMin < 0 ? 0 : yMin;
	yMax = yMax > MAX_Y_TILE ? MAX_Y_TILE : yMax;

	for (int x = xMin; x < xMax; x++)
	{
		for (int y = yMin; y < yMax; y++)
		{
			if (map[x][y] == 0)
				validPosList.push_back(*new Vector2D(x, y));
		}
	}
	if (!validPosList.empty())
	{		
		int index = rand() % validPosList.size(); // pick a random index
		*pos = validPosList[index]; // a random value taken from that list
		return true;
	}
	else
	{
		GAMELOG("Valid list is empty!");
	}
	return false;
}

std::vector<Vector2D> AStar::GetListVecInAxisValid(int x, int y, int xRound, int yRound, int counBrick)
{
	try
	{
		std::vector<Vector2D> validList;
		
		int _countBrick = 0;

		int _dtX = xRound > x ? 1 : -1;
		while (xRound != x && _countBrick <= counBrick)
		{
			if (isValid(x, y))
			{
				if (!isObstacle(x, y, map)) {
					validList.push_back(*new Vector2D(x, y));
				}
				else
				{
					_countBrick++;
				}
			}

			x += _dtX;
		}

		_countBrick = 0;
		int _dtY = yRound > y ? 1 : -1;
		while (yRound != y && _countBrick <= counBrick)
		{
			if (isValid(x, y))
			{
				if (!isObstacle(x, y, map)) {
					validList.push_back(*new Vector2D(x, y));
				}
				else
				{
					_countBrick++;
				}
			}
			y += _dtY;
		}

		if (validList.empty())
		{
			return GetListVecInAxisValid(x, y, xRound, yRound, counBrick + 1);
		}
		return validList;
	}
	catch (exception e)
	{
		GAMELOG(e.what());
	}
}

double calculateVecH(Vector2D vec0, Vector2D vec1)
{
	double H = (sqrt((vec0.x - vec1.x)*(vec0.x - vec1.x)
		+ (vec0.y - vec1.y))*(vec0.y - vec1.y));
	return H;
}

Vector2D* AStar::RandomVecInAxisValid(Vector2D vecMine, Vector2D vecOther, int range)
{
	int xRound = vecMine.x < vecOther.x ? vecMine.x - range : vecOther.x + range;
	int yRound = vecMine.y < vecOther.y ? vecMine.y - range : vecMine.y + range;
	std::vector<Vector2D> validList = GetListVecInAxisValid(vecOther.x, vecOther.y, xRound, yRound, 0);
	Vector2D* vecResult = new Vector2D(vecOther.x, vecOther.y);
	double dirResult;
	if (!validList.empty())
	{
		vecResult->x = validList.at(0).x;
		vecResult->y = validList.at(0).y;
		dirResult = calculateVecH(vecMine, *vecResult);
		for (auto vec : validList)
		{
			if (calculateVecH(vecMine, vec) < dirResult)
			{
				vecResult->x = vec.x;
				vecResult->y = vec.y;
				dirResult = calculateVecH(vecMine, vec);
			}
		}
	}
	validList.clear();
	return vecResult;
}

TilePart AStar::GetNextPart(Vector2D beginPart, Vector2D destPart)
{
	TilePart resuit;

	int dx = destPart.x - beginPart.x;
	int dy = destPart.y - beginPart.y;

	float absTan = dx == 0 ? dy : abs(dy / dx);

	if (dx > 0)
	{
		if (dy > 0)
		{
			resuit.dir = PD_Left_Top;
			//[x+1, y] [x+1,y+1] [x, y+1]
			if (absTan <= 0.33f)
			{
				//[x+1, y]
				resuit.vec.x = beginPart.x + 1;
				resuit.vec.y = beginPart.y;
			}
			else if (absTan <= 3)
			{
				//[x+1,y+1]
				resuit.vec.x = beginPart.x + 1;
				resuit.vec.y = beginPart.y + 1;
			}
			else
			{
				//[x, y+1]
				resuit.vec.x = beginPart.x;
				resuit.vec.y = beginPart.y + 1;
			}
		}
		else
		{
			resuit.dir = PD_Left_Bottom;
			//[x+1, y] [x+1,y-1] [x, y-1]
			if (absTan <= 0.33f)
			{
				//[x+1, y]
				resuit.vec.x = beginPart.x + 1;
				resuit.vec.y = beginPart.y;
			}
			else if (absTan <= 3)
			{
				//[x+1,y-1]
				resuit.vec.x = beginPart.x + 1;
				resuit.vec.y = beginPart.y - 1;
			}
			else
			{
				//[x, y-1]
				resuit.vec.x = beginPart.x;
				resuit.vec.y = beginPart.y - 1;
			}
		}
	}
	else
	{
		if (dy > 0)
		{
			resuit.dir = PD_Right_Top;
			//[x-1, y] [x-1,y+1] [x, y+1]
			if (absTan <= 0.33f)
			{
				//[x-1, y]
				resuit.vec.x = beginPart.x - 1;
				resuit.vec.y = beginPart.y;
			}
			else if (absTan <= 3)
			{
				//[x-1,y+1]
				resuit.vec.x = beginPart.x - 1;
				resuit.vec.y = beginPart.y + 1;
			}
			else
			{
				//[x, y+1]
				resuit.vec.x = beginPart.x;
				resuit.vec.y = beginPart.y + 1;
			}
		}
		else
		{
			resuit.dir = PD_Right_Bottom;
			//[x-1, y] [x-1,y-1] [x, y -1]
			if (absTan < 0.33f)
			{
				//[x-1, y]
				resuit.vec.x = beginPart.x - 1;
				resuit.vec.y = beginPart.y;
			}
			else if (absTan < 3)
			{
				//[x-1,y-1]
				resuit.vec.x = beginPart.x - 1;
				resuit.vec.y = beginPart.y - 1;
			}
			else
			{
				//[x, y-1]
				resuit.vec.x = beginPart.x;
				resuit.vec.y = beginPart.y - 1;
			}
		}
	}

	return resuit;
}

std::vector<Node> AStar::findPathNodeV2(Node player, Node dest, int size_part)
{
	if (abs(player.x - dest.x) <= size_part && abs(player.y - dest.y) <= size_part)
	{
		Vector2D partPlayer, partDest;
		partPlayer.x = player.x / size_part;
		partPlayer.y = player.y / size_part;

		partDest.x = dest.x / size_part;
		partDest.y = dest.y / size_part;

		return findPathV2WithPart(player, dest, partPlayer, partDest, size_part).path;
	}

	Vector2D partPlayer, partDest;
	partPlayer.x = player.x / size_part;
	partPlayer.y = player.y / size_part;

	partDest.x = dest.x / size_part;
	partDest.y = dest.y / size_part;

	if (partPlayer.x == partDest.x && partPlayer.y == partDest.y)
	{
		return findPathV2WithPart(player, dest, partPlayer, partDest, size_part).path;
	}
	else 
	{
		Node next;
		TilePart partNext = GetNextPart(partPlayer, partDest);

		switch (partNext.dir)
		{
		case PD_Left_Top:
			printLog("Move left top");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part; x <= partNext.vec.x * size_part + i; x++)
				{
					for (int y = partNext.vec.y * size_part + i; y >= partNext.vec.y * size_part; y--)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}

		case PD_Left_Bottom:
			printLog("Move left bottom");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part + i; x >= partNext.vec.x * size_part; x--)
				{
					for (int y = partNext.vec.y * size_part; y <= partNext.vec.y * size_part + i; y++)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}

		case PD_Right_Top:
			printLog("Move right top");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part - i; x <= partNext.vec.x * size_part; x++)
				{
					for (int y = partNext.vec.y * size_part; y <= partNext.vec.y * size_part + i; y++)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}

		case PD_Right_Bottom:
			printLog("Move right bottom");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part; x >= partNext.vec.x * size_part - i; x--)
				{
					for (int y = partNext.vec.y * size_part - i; y <= partNext.vec.y * size_part; y++)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}
		default:
			break;
		}

		return findPathNodeV2(player, dest, size_part + PART_SIZE);
	}

}

AstarResult AStar::findPathV2(Node player, Node dest)
{
	AstarResult resuit;
	resuit.path = findPathNodeV2(player, dest, PART_SIZE);
	return resuit;
}

AstarResult AStar::findPathV2WithPart(Node begin, Node dest, Vector2D partPlayer, Vector2D partDest, int size_part)
{
	AstarResult resuit;

	if (!isValid(dest.x, dest.y)) {
		std::cout << "Destination is not valid" << std::endl;
		return resuit;
	}

	if (isObstacle(dest.x, dest.y, map))
	{
		std::cout << "Destination is an obstacle" << std::endl;
		return resuit;
	}

	if (isDestination(begin.x, begin.y, dest)) {
		std::cout << "You are the destination" << std::endl;
		return resuit;
	}

	int xMin, xMax, yMin, yMax;

	if (partPlayer.x < partDest.x) {
		xMin = partPlayer.x * size_part;
		xMax = (partDest.x + 1)*size_part;
	}
	else
	{
		xMin = partDest.x * size_part;
		xMax = (partPlayer.x + 1)*size_part;
	}

	if (partPlayer.y < partDest.y) {
		yMin = partPlayer.y * size_part;
		yMax = (partDest.y + 1)*size_part;
	}
	else
	{
		yMin = partDest.y * size_part;
		yMax = (partPlayer.y + 1)*size_part;
	}

	if (xMax > MAX_X_TILE) xMax = MAX_X_TILE;
	if (yMax > MAX_Y_TILE) yMax = MAX_Y_TILE;

	//Initialize whole map
	std::array<std::array < Node, MAX_Y_TILE>, MAX_X_TILE> _mapNode;
	bool closedList[MAX_Y_TILE][MAX_X_TILE];

	while (size_part < MAX_Y_TILE / 2)
	{
		for (int x = xMin; x < xMax; x++) {
			for (int y = yMin; y < yMax; y++) {
				_mapNode[x][y].fCost = FLT_MAX;
				_mapNode[x][y].gCost = FLT_MAX;
				_mapNode[x][y].hCost = FLT_MAX;
				_mapNode[x][y].parentX = -1;
				_mapNode[x][y].parentY = -1;
				_mapNode[x][y].x = x;
				_mapNode[x][y].y = y;

				closedList[x][y] = false;
			}
		}

		//Initialize our starting list
		int x = begin.x;
		int y = begin.y;
		_mapNode[x][y].fCost = 0.0;
		_mapNode[x][y].gCost = 0.0;
		_mapNode[x][y].hCost = 0.0;
		_mapNode[x][y].parentX = x;
		_mapNode[x][y].parentY = y;

		resuit.openList.emplace_back(_mapNode[x][y]);

		bool destinationFound = false;


#pragma region  Open fibonacciheap
		FibonacciHeap<Node> openList;

		openList.insert(_mapNode[x][y]);

		while (!openList.isEmpty() && openList.size() < MAX_X_TILE * MAX_Y_TILE)
		{
			Node node;
			do
			{
				node = openList.removeMinimum();

			} while (!openList.isEmpty() && !isValidAndNotObs(node.x, node.y));

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
					if (isValid(x + newX, y + newY))
					{
						if (isDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0))
						{
							//Destination found - make path.
							_mapNode[x + newX][y + newY].parentX = x;
							_mapNode[x + newX][y + newY].parentY = y;
							destinationFound = true;
							resuit.path = makePath(_mapNode, dest);
							return resuit;
						}
						else if (!closedList[x + newX][y + newY])
						{
							if (newX == 0 || newY == 0)
								gNew = node.gCost + 4.5f;
							else
								continue;
							hNew = calculateH(x + newX, y + newY, dest);
							fNew = gNew + hNew;

							//Check if this path is better than the one already present.
							if (_mapNode[x + newX][y + newY].fCost == FLT_MAX || _mapNode[x + newX][y + newY].fCost > fNew)
							{
								_mapNode[x + newX][y + newY].fCost = fNew;
								_mapNode[x + newX][y + newY].gCost = gNew;
								_mapNode[x + newX][y + newY].hCost = hNew;
								_mapNode[x + newX][y + newY].parentX = x;
								_mapNode[x + newX][y + newY].parentY = y;
								openList.insert(_mapNode[x + newX][y + newY]);
							}
						}
					}
				}
			}
		}

#pragma endregion

		if (destinationFound == false) {
			std::cout << "Destination not found" << std::endl;
			return resuit;
		}

		size_part += PART_SIZE;
	}

	return resuit;
}