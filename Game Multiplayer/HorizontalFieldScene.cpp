#include <algorithm>

#include "HorizontalFieldScene.h"
#include "PairNodes.h"
#include "AStar.h"

HorizontalFieldScene::HorizontalFieldScene()
{
	_map = new GameMap("Resource files/ai_map2.tmx");

	_player = new Player();
	_player->Position = D3DXVECTOR2(48.f, 48.f);

	begin = GridTile();
	/*begin.SetPosition(D3DXVECTOR2(0.f, 0.f));
	begin.SetX(0);
	begin.SetY(0);
	begin.SetType(Destination);

	destination = GridTile();
	destination.SetPosition(D3DXVECTOR2(0.f, 0.f));
	destination.SetX(WIDTH / X_STEP - 1);
	destination.SetY(HEIGHT / Y_STEP - 1);*/
	destination.SetType(Destination);

	destinationNode = new GridTile();
	*destinationNode = destination;

	camera.Init(GameGlobal::Height, GameGlobal::Width, 0, 10000, 0, 10000);
	camera.setPosition(D3DXVECTOR2(0.f, 0.f));

	D3DXVECTOR2 offset(GameGlobal::Width / 2.f - camera.Position.x, GameGlobal::Height / 2.f - camera.Position.y);

	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			gridMap[x][y] = new GridTile();
			/*gridMap[x][y]->x = x;
			gridMap[x][y]->y = y;*/
			gridMap[x][y]->position = D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP);

			//Add obstacles to a drawing list.
			/*if (_map->obstaclesNodes.count(x + y * (WIDTH / X_STEP)))
			{
				GridTile* tile = gridMap[x][y];
				tile->SetType(Obstacle);
				drawingObstacles.emplace_back(tile);				
			}		*/		
		}
	}

	NPC* npc1 = new NPC();
	npc1->Position = D3DXVECTOR2(912.f, 48.f);
	_npcList.emplace_back(npc1);

	NPC* npc2 = new NPC();
	npc2->Position = D3DXVECTOR2(912.f, 432.f);
	_npcList.emplace_back(npc2);

	for (int counter = 0; counter < _npcList.size(); counter++)
	{
		Bullet* bullet = new Bullet();
		_bulletList.emplace_back(bullet);
	}

	/*for (int index = 0; index < 2; index++)
	{
		NPC* npc = new NPC();
		_npcList.emplace_back(npc);
	}*/

	UpdateTankNodes();
}

void HorizontalFieldScene::Update(float dt)
{
	if (keyboard[0x31]) // 1 -> change drawing mode.
	{
		_isDrawDestinationOnly = !_isDrawDestinationOnly;
		if (_isDrawMap) GAMELOG("Drawing destination only."); else GAMELOG("Drawing all node.");
		keyboard[0x31] = false;
	}
	else if (keyboard[0x32]) // 2 -> make the first npc in npcList fire.
	{
		//_npcList[0]->Fire();
		keyboard[0x32] = false;
	}
	else if (keyboard[0x33]) // 3 -> print bullet position.
	{
		GAMELOG("Bullet pos: %f, %f", _bulletList[0]->Position.x, _bulletList[0]->Position.y);
		keyboard[0x33] = false;
	}

	if (keyboard[0x51]) // Q
	{
		_isDrawPath = !_isDrawPath;
		if (_isDrawPath) GAMELOG("Drawing path on."); else GAMELOG("Drawing path off.");
		keyboard[0x51] = false;
	}
	else if (keyboard[0x57]) // W
	{
		_isDrawMap = !_isDrawMap;
		if (_isDrawMap) GAMELOG("Drawing map on."); else GAMELOG("Drawing map off.");
		keyboard[0x57] = false;
	}
	else if (keyboard[0x45]) // E
	{
		_isDrawObstacles = !_isDrawObstacles;
		if (_isDrawObstacles) GAMELOG("Drawing obstacles on."); else GAMELOG("Drawing obstacles off.");
		keyboard[0x45] = false;
	}
	else if (keyboard[0x52]) // R
	{
		ResetScene();
	}
	else if (keyboard[0x54]) // T
	{
		//Make NPC move.
		keyboard[0x54] = false;
	}
	else if (keyboard[0x46]) // F
	{
		_isMouseActive = !_isMouseActive;
		if (_isMouseActive)
			GAMELOG("Mouse is actived!");
		else
			GAMELOG("Mouse is deactived!");
		keyboard[0x46] = false;
	}
	else if (keyboard[0x25]) // Left
	{
		camera.Position.x += 32.f;
		keyboard[0x25] = false;
	}
	else if (keyboard[0x26]) // Up
	{
		camera.Position.y += 32.f;
		keyboard[0x26] = false;
	}
	else if (keyboard[0x27]) // Right
	{
		camera.Position.x -= 32.f;
		keyboard[0x27] = false;
	}
	else if (keyboard[0x28]) // Down
	{
		camera.Position.y -= 32.f;
		keyboard[0x28] = false;
	}

	for (auto bullet : _bulletList)
		bullet->Update(dt);
	
	//Check collision with bricks.
	/*for (auto brick : _map->getBrickList())
	{
		if (!brick->IsDeleted)
		{
			_player->CheckCollision(brick);
			
			for (auto npc : _npcList)
			{
				npc->CheckCollision(brick);
			}

			for (auto bullet : _bulletList)
				bullet->CheckCollision(brick);
		}
	}*/

	for (auto npc : _npcList)
	{
		/*if (!npc->hasPath)
		{			
			GiveNPCPath(npc);
			npc->hasPath = true;
		}

		for (auto bullet : _bulletList)
			bullet->CheckCollision(npc);

		for (auto cNpc : _npcList)
		{
			if (cNpc == npc)
				continue;
			if (npc->GetCollisionResult(cNpc))
			{
				NPC* changePathNPC = FindChangePathNPC(npc, cNpc);

				if (changePathNPC->IsPause())
					continue;

				if ((cNpc->IsStand() && npc->isChangedPath) || (npc->IsStand() && cNpc->isChangedPath))
				{
					UpdateTankNodes();					
				}
				
				GridTile* tile = nullptr;
				do
				{
					Direction d = changePathNPC->GetNewDirection();
					tile = GetNPCTempPoint(changePathNPC, d);
					if (tile == nullptr)
						changePathNPC->changePathTime++;
				} while (tile == nullptr && changePathNPC->changePathTime < 3);				

				if (std::find(changePathNPC->tempPath.begin(),
					changePathNPC->tempPath.end(),
					tile) != changePathNPC->tempPath.end()
					|| changePathNPC->tempPath.empty())
				{
					changePathNPC->ChangePath(tile);
					changePathNPC->isChangedPath = true;
				}

				if (changePathNPC == npc)
					cNpc->Pause();
				else
					npc->Pause();
			}				
		}		

		if (npc->GetCollisionResult(_player))
		{
			npc->isChangedPath = true;
			GridTile* tile = nullptr;
			do
			{
				if (npc->IsPause())
				{
					_player->Pause(0.5f);
					continue;
				}
					
				Direction d = npc->GetNewDirection();
				tile = GetNPCTempPoint(npc, d);
				if (tile == nullptr)
					npc->changePathTime++;
			} while (tile == nullptr && npc->changePathTime < 3);

			if (std::find(npc->tempPath.begin(),
				npc->tempPath.end(),
				tile) != npc->tempPath.end()
				|| npc->tempPath.empty())
			{
				npc->ChangePath(tile);
			}

			_player->Pause(0.65f);
		}*/

		npc->Update(dt);
	}

	GivePlayerOrder();
	for (auto bullet : _bulletList)
		bullet->CheckCollision(_player);
	_player->Update(dt);
}

void HorizontalFieldScene::OnLeftMouseDown(float x, float y)
{
	if (!_isMouseActive) return;

	GAMELOG("(%d, %d)", int(x / float(X_STEP)), int(y / float(Y_STEP)));
}

void HorizontalFieldScene::OnRightMouseDown(float x, float y)
{
	D3DXVECTOR2 offset(GameGlobal::Width / 2.f - camera.Position.x, GameGlobal::Height / 2.f - camera.Position.y);

	//Adjust x and y coordinate with offset when camera moves.
	x -= offset.x;
	y -= offset.y;
	if (!_isMouseActive) return;

	bool isInvalidDestination = false;
	
	if (!_isPlayerMoving)
	{
		ResetScene();
		/*if (gridMap[begin.GetX()][begin.GetY()]->type != Obstacle)
			gridMap[begin.GetX()][begin.GetY()]->SetType(Empty);
		begin.SetPosition(_player->Position);
		begin.x = int(begin.position.x / float(X_STEP));
		begin.y = int(begin.position.y / float(Y_STEP));
		gridMap[begin.x][begin.y]->SetType(Begin);

		if (gridMap[int(x / float(X_STEP))][int(y / float(Y_STEP))]->type == Obstacle) isInvalidDestination = true;

		if (gridMap[destination.GetX()][destination.GetY()]->type != Obstacle)
			gridMap[destination.GetX()][destination.GetY()]->SetType(Empty);

		destination.SetPosition(D3DXVECTOR2(x, y));
		destination.x = int(x / float(X_STEP));
		destination.y = int(y / float(Y_STEP));
		*destinationNode = destination;
		gridMap[destination.x][destination.y]->SetType(Destination);*/

		UpdateTankNodes();
		FindPath();
		DrawPath();
		//if (isInvalidDestination) gridMap[destination.x][destination.y]->SetType(Obstacle);
		_isPlayerMoving = true;
	}
	else
	{
		_player->Stop();
		_isPlayerMoving = false;
		_player->currentNodeIndex = 0;

		ResetScene();

		/*if (gridMap[begin.GetX()][begin.GetY()]->type != Obstacle)
			gridMap[begin.GetX()][begin.GetY()]->SetType(Empty);

		begin.SetPosition(_player->Position);
		begin.x = int(begin.position.x / float(X_STEP));
		begin.y = int(begin.position.y / float(Y_STEP));
		gridMap[begin.x][begin.y]->SetType(Begin);

		if (gridMap[int(x / float(X_STEP))][int(y / float(Y_STEP))]->type == Obstacle) isInvalidDestination = true;

		if (gridMap[destination.GetX()][destination.GetY()]->type != Obstacle)
			gridMap[destination.GetX()][destination.GetY()]->SetType(Empty);

		destination.SetPosition(D3DXVECTOR2(x, y));
		destination.x = int(x / float(X_STEP));
		destination.y = int(y / float(Y_STEP));
		*destinationNode = destination;
		gridMap[destination.x][destination.y]->SetType(Destination);*/

		UpdateTankNodes();
		FindPath();
		DrawPath();
		//if (isInvalidDestination) gridMap[destination.x][destination.y]->SetType(Obstacle);
		_isPlayerMoving = true;
	}
}

void HorizontalFieldScene::ResetScene()
{
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			if (gridMap[x][y]->type != Obstacle)
				gridMap[x][y]->SetType(Empty);
			else
				gridMap[x][y]->SetType(Obstacle);
		}
	}
	_player->path.clear();
	drawingPath.clear();
}

void HorizontalFieldScene::FindPath()
{
	//CoorNode cbegin = CoorNode(begin.GetX(), begin.GetY());
	//CoorNode cdes = CoorNode(destination.GetX(), destination.GetY());
	//PairNode pair(cbegin, cdes);

	//std::unordered_map<PairNode, std::vector<GridTile*>>::iterator foundPath = storedPath.find(pair);
	//
	//if (foundPath != storedPath.end())
	//{
	//	_player->path.clear();
	//	_player->path = foundPath->second;
	//	
	//	if (foundPath->first.IsReversed(pair))			
	//	{
	//		//Reverse path.
	//		std::reverse(_player->path.begin(), _player->path.end());
	//	}
	//}
	//else
	//{
	//	RunAStar();
	//}
}

void HorizontalFieldScene::FindPath(NPC* npc, Node destination)
{
	Node beginNode;
	//Setting position for begin node.	
	beginNode.SetX(int(npc->Position.x / float(X_STEP)));
	beginNode.SetY(int(npc->Position.y / float(Y_STEP)));

	CoorNode cbegin = CoorNode(beginNode.GetX(), beginNode.GetY());
	CoorNode cdes = CoorNode(destination.GetX(), destination.GetY());
	PairNode pair = PairNode(cbegin, cdes);

	std::unordered_map<PairNode, std::vector<GridTile*>>::iterator foundPath = storedPath.find(pair);
	if (foundPath != storedPath.end())
	{
		//npc->path.clear();
		//npc->path = foundPath->second;

		//if (foundPath->first.IsReversed(pair))
		//{
		//	//Reverse path.
		//	std::reverse(npc->path.begin(), npc->path.end());
		//}
	}
	else
	{
		tankNodes.erase(cbegin.GetX() + cbegin.GetY() * (WIDTH / X_STEP));
		RunAStar(npc, beginNode, destination);		
	}
}

void HorizontalFieldScene::RunAStar()
{
	//_player->path.clear();
	//Node beginNode;
	////Setting position for begin node.
	//beginNode.SetX(begin.GetX());
	//beginNode.SetY(begin.GetY());

	//Node destNode;
	//destNode.SetX(destination.GetX());
	//destNode.SetY(destination.GetY());

	//int id = beginNode.GetX() + beginNode.GetY() * (WIDTH / X_STEP);
	//tankNodes.erase(id);

	///*std::vector<Node> result = AStar::aStar(beginNode, destNode, gridMap, _map->obstaclesNodes, tankNodes);

	//for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
	//{
	//	_player->path.emplace_back(gridMap[it->GetX()][it->GetY()]);
	//}*/

	//PairNode pair = PairNode(CoorNode(begin.GetX(), begin.GetY()),
	//	CoorNode(destination.GetX(), destination.GetY()));
	//storedPath.emplace(pair, _player->path);
}

void HorizontalFieldScene::RunAStar(NPC* npc, Node beginNode, Node destination)
{
	//npc->path.clear();	

	/*std::vector<Node> result = AStar::aStar(beginNode, destination, gridMap, _map->obstaclesNodes, tankNodes);

	for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
	{
		npc->path.emplace_back(gridMap[it->GetX()][it->GetY()]);
	}*/

	PairNode pair = PairNode(CoorNode(beginNode.GetX(), beginNode.GetY()),
		CoorNode(destination.GetX(), destination.GetY()));
	//storedPath.emplace(pair, npc->path);
}

void HorizontalFieldScene::DrawPath()
{
	if (!_player->path.empty())
		for (int index = 0; index < _player->path.size(); index++)
		{
			//path[index]->SetType(Path);
			GridTile* tile = _player->path[index];
			drawingPath.emplace_back(tile);
			drawingPath[index]->SetType(Path);
		}
	else
		GAMELOG("Path is empty!");
}

void HorizontalFieldScene::GivePlayerOrder()
{
	/*if (_player->IsPause())
		return;*/

	if (_isPlayerMoving)
	{
		if (_player->currentNodeIndex < _player->path.size())
		{
			_player->Move(_player->path[_player->currentNodeIndex]->position);
			if (!_player->IsMoving()) _player->currentNodeIndex++;
		}
		else
		{
			_player->currentNodeIndex = 0;
			_isPlayerMoving = false;
		}
	}
}

void HorizontalFieldScene::GiveNPCPath(NPC* npc)
{	
	int x, y;
	//if (npc->isRepath)
	//{
	//	/*x = npc->path[npc->path.size() - 1]->GetX();
	//	y = npc->path[npc->path.size() - 1]->GetY();*/
	//	if (npc->Position == npc->path[npc->currentNodeIndex]->position)
	//		npc->currentNodeIndex = 1;
	//	npc->isRepath = false;
	//}
	//else
	//{
	//	//Generate npc destination
	//	do
	//	{
	//		x = rand() % 30;
	//		y = rand() % 15;
	//	} while (!IsValidNPCDestination(x, y));	
	//}

	Node npcDes;
	/*npcDes.SetX(x); npcDes.SetY(y);*/
	UpdateTankNodes();
	FindPath(npc, npcDes);
}

NPC* HorizontalFieldScene::FindChangePathNPC(NPC* npc1, NPC* npc2)
{
	/*if (npc1->isChangedPath)
		return npc1;
	if (npc2->isChangedPath)
		return npc2;

	if (npc1->currentNodeIndex >= npc1->path.size())
		npc1->currentNodeIndex = npc1->path.size() - 1;
	if (npc2->currentNodeIndex >= npc2->path.size())
		npc2->currentNodeIndex = npc2->path.size() - 1;

	GridTile* tile1 = npc1->path[npc1->currentNodeIndex];
	GridTile* tile2 = npc2->path[npc2->currentNodeIndex];

	float dx1 = std::abs(npc1->Position.x - tile1->position.x);
	float dx2 = std::abs(npc2->Position.x - tile2->position.x);
	float dy1 = std::abs(npc1->Position.y - tile1->position.y);
	float dy2 = std::abs(npc2->Position.y - tile2->position.y);

	float d1 = (dx1 != 0) ? dx1 : dy1;
	float d2 = (dx2 != 0) ? dx2 : dy2;	

	if (d1 > d2) return npc1;
	else return npc2;*/
	return nullptr;
}

GridTile* HorizontalFieldScene::GetNPCTempPoint(NPC* npc, Direction direction)
{
	UpdateTankNodes();
	int _x = int(npc->Position.x / float(X_STEP));
	int _y = int(npc->Position.y / float(Y_STEP));

	switch (direction)
	{	
	case D_Left:
		if (IsValidNPCDestination(_x - 1, _y))
			return gridMap[_x - 1][_y];
		else
			return nullptr;
		break;
	case D_Right:
		if (IsValidNPCDestination(_x + 1, _y))
			return gridMap[_x + 1][_y];
		else
			return nullptr;
		break;
	case D_Up:
		if (IsValidNPCDestination(_x, _y - 1))
			return gridMap[_x ][_y - 1];
		else
			return nullptr;
		break;
	case D_Down:
		if (IsValidNPCDestination(_x, _y + 1))
			return gridMap[_x][_y + 1];
		else
			return nullptr;
		break;
	default:
		break;
	}
}

bool HorizontalFieldScene::IsValidNPCDestination(int x, int y)
{
	int id = x + y * (WIDTH / X_STEP);

	/*if (!_map->obstaclesNodes.count(id) && !tankNodes.count(id)&& !npcsDestination.count(id))
	{
		if (x < 0 || y < 0 || x >= (WIDTH / X_STEP) || y >= (HEIGHT / Y_STEP))
			return false;
		return true;
	}*/
	return false;
}

void HorizontalFieldScene::UpdateTankNodes()
{
	tankNodes.clear();
	drawingTankNodes.clear();
	int _x = int(_player->Position.x / float(X_STEP)), _y = int(_player->Position.y / float(Y_STEP));
	int id = _x + _y * (WIDTH / X_STEP);
	tankNodes.insert(id);

	GridTile* tile = gridMap[_x][_y];
	tile->SetType(Obstacle);
	drawingTankNodes.emplace_back(tile);

	for (auto npc : _npcList)
	{
		_x = int(npc->Position.x / float(X_STEP));
		_y = int(npc->Position.y / float(Y_STEP));
		id = _x + _y * (WIDTH / X_STEP);
		tankNodes.insert(id);

		GridTile* tile1 = gridMap[_x][_y];
		tile->SetType(Obstacle);
		drawingTankNodes.emplace_back(tile1);
	}	
}

Bullet* HorizontalFieldScene::GetBullet()
{
	for (int index = 0; index < _bulletList.size(); index++)
		if (_bulletList[index]->IsDeleted)
			return _bulletList[index];
	
	return nullptr;
}

void HorizontalFieldScene::Draw()
{
	D3DXVECTOR2 offset(GameGlobal::Width / 2.f - camera.Position.x, GameGlobal::Height / 2.f - camera.Position.y);
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			if (_isDrawDestinationOnly)
			{
				if (gridMap[x][y]->type == Destination)
					gridMap[x][y]->Draw(offset);
			}
			else
				gridMap[x][y]->Draw(offset);
		}
	}

	if (_isDrawMap)
		_map->Draw(camera);

	if (_isDrawObstacles)
	{
		for (int index = 0; index < drawingObstacles.size(); index++)
			drawingObstacles[index]->Draw(offset);
		for (int index = 0; index < drawingTankNodes.size(); index++)
			drawingTankNodes[index]->Draw(offset);
	}
		

	if (_isDrawPath)
		for (int index = 0; index < drawingPath.size(); index++)
			drawingPath[index]->Draw(offset);
	
	destinationNode->Draw(offset);

	for (auto npc : _npcList)
	{
		npc->Draw(offset);
	}

	_player->Draw(offset);

	for (auto bullet : _bulletList)
		bullet->Draw(offset);
}