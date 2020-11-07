#include "HorizontalFieldScene.h"
#include "AStar.h"

HorizontalFieldScene::HorizontalFieldScene()
{
	_map = new GameMap("Resource files/ai_map1.tmx");
	obstaclesNode = _map->obstaclesNode;
	_player = new Player();
	_player->Position = D3DXVECTOR2(48.f, 48.f);

	begin = GridTile();
	begin.SetPosition(D3DXVECTOR2(0.f, 0.f));
	begin.SetX(0);
	begin.SetY(0);
	begin.SetType(Destination);

	destination = GridTile();
	destination.SetPosition(D3DXVECTOR2(0.f, 0.f));
	destination.SetX(WIDTH / X_STEP - 1);
	destination.SetY(HEIGHT / Y_STEP - 1);
	destination.SetType(Destination);

	destinationNode = new GridTile();
	*destinationNode = destination;

	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			gridMap[x][y] = new GridTile();
			gridMap[x][y]->x = x;
			gridMap[x][y]->y = y;
			gridMap[x][y]->position = D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP);

			//Change value of obstacles node.
			if (obstaclesNode.count(x + y * (WIDTH / X_STEP)))
				gridMap[x][y]->SetType(Obstacle);
		}
	}
}

void HorizontalFieldScene::Update(float dt)
{
	if (keyboard[0x31]) // 1 -> normal mode, right click to set destination.
	{
		
		keyboard[0x31] = false;
	}
	else if (keyboard[0x32]) // 2 -> auto mode, right click will auto find path and move tank to destination.
	{
		
		keyboard[0x32] = false;
	}
	else if (keyboard[0x33]) // 3 -> editor mode, left click will set obstacle, right click will remove obstacle.
	{
		
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
		
		keyboard[0x45] = false;
	}
	else if (keyboard[0x52]) // R
	{
		ResetScene();
	}
	else if (keyboard[0x54]) // T
	{
		
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
	
	//Check collision with bricks.
	for (auto brick : _map->getBrickList())
	{
		if (!brick->IsDeleted)
		{
			_player->CheckCollision(brick);
			
			/*for (auto npc : _npcList)
			{
				npc->CheckCollision(brick);
			}*/
		}
	}

	GivePlayerOrder();
	_player->Update(dt);
}

void HorizontalFieldScene::OnLeftMouseDown(float x, float y)
{
	if (!_isMouseActive) return;

	if (mode != 3)
	{
		GAMELOG("(%d, %d)", int(x / float(X_STEP)), int(y / float(Y_STEP)));
	}
	else
	{
		int _x = int(x / float(X_STEP)), _y = int(y / float(Y_STEP));
		int id = _x + _y * (WIDTH / X_STEP);
		_map->obstaclesNode.insert(id);
		gridMap[_x][_y]->SetType(Obstacle);
		GAMELOG("%f, %f", x, y);
		GAMELOG("(%d, %d)", _x, _y);
	}
}

void HorizontalFieldScene::OnRightMouseDown(float x, float y)
{
	if (!_isMouseActive) return;

	bool isInvalidDestination = false;

	if (!_isPlayerMoving)
	{
		ResetScene();
		if (gridMap[begin.GetX()][begin.GetY()]->type != Obstacle)
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
		gridMap[destination.x][destination.y]->SetType(Destination);

		RunAStar();
		DrawPath();
		if (isInvalidDestination) gridMap[destination.x][destination.y]->SetType(Obstacle);
		_isPlayerMoving = true;
	}
	else
	{
		_player->Stop();
		_isPlayerMoving = false;
		currentNodeIndex = 0;

		ResetScene();

		if (gridMap[begin.GetX()][begin.GetY()]->type != Obstacle)
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
		gridMap[destination.x][destination.y]->SetType(Destination);

		RunAStar();
		DrawPath();
		if (isInvalidDestination) gridMap[destination.x][destination.y]->SetType(Obstacle);
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
	drawPath.clear();
}

void HorizontalFieldScene::RunAStar()
{
	_player->path.clear();
	Node beginNode;
	//Setting position for begin node.
	beginNode.SetX(begin.GetX());
	beginNode.SetY(begin.GetY());

	Node destNode;
	destNode.SetX(destination.GetX());
	destNode.SetY(destination.GetY());
	obstaclesNode = _map->obstaclesNode;
	std::vector<Node> result = AStar::aStar(beginNode, destNode, gridMap);
	for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
	{
		_player->path.push_back(gridMap[it->GetX()][it->GetY()]);
	}
}

void HorizontalFieldScene::DrawPath()
{
	if (!_player->path.empty())
		for (int index = 0; index < _player->path.size(); index++)
		{
			//path[index]->SetType(Path);
			GridTile* tile = _player->path[index];
			drawPath.emplace_back(tile);
			drawPath[index]->SetType(Path);
		}
	else
		GAMELOG("Path is empty!");
}

void HorizontalFieldScene::GivePlayerOrder()
{
	if (_isPlayerMoving)
	{
		if (currentNodeIndex < _player->path.size())
		{
			_player->Move(_player->path[currentNodeIndex]->position);
			if (!_player->IsMoving()) currentNodeIndex++;
		}
		else
		{
			currentNodeIndex = 0;
			_isPlayerMoving = false;
		}
	}
}

void HorizontalFieldScene::Draw()
{
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			gridMap[x][y]->Draw();
		}
	}

	if (_isDrawMap)
		_map->Draw();

	if (_isDrawPath)
		for (int index = 0; index < drawPath.size(); index++)
			drawPath[index]->Draw();
	
	destinationNode->Draw();
	_player->Draw();
}