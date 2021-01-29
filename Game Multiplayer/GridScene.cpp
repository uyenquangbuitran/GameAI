#include "GridScene.h"
#include "SceneManager.h"
#include "AStar.h"
#include <math.h>

GridScene::GridScene()
{
	player = new Player();
	player->Position = D3DXVECTOR2(16.f, 16.f);

	/*begin = GridTile();
	begin.SetPosition(D3DXVECTOR2(0.f, 0.f));	
	begin.SetX(0);
	begin.SetY(0);
	begin.SetType(Destination);

	destination = GridTile();
	destination.SetPosition(D3DXVECTOR2(0.f, 0.f));
	destination.SetX(WIDTH / X_STEP - 1);
	destination.SetY(HEIGHT / Y_STEP - 1);
	destination.SetType(Destination);*/

	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{			
			map[x][y] = new GridTile();
			/*map[x][y]->x = x;
			map[x][y]->y = y;*/
			map[x][y]->position = D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP);

			/*
			int id = x + y * (WIDTH / X_STEP);
			if (id % 2 == 0)
			{
				if (y % 2 == 0)
					map[x][y]->SetType(Debug0);
				else
					map[x][y]->SetType(Debug1);
			}
			else
			{
				if (y % 2 == 0)
					map[x][y]->SetType(Debug1);
				else
					map[x][y]->SetType(Debug0);
			}
			*/

			//Change value of obstacles node.
			/*if (obstaclesNode.count(x + y * (WIDTH / X_STEP)))
				map[x][y]->SetType(Obstacle);*/
		}
	}
}

void GridScene::Update(float dt)
{
	if (keyboard[0x31]) // 1 -> normal mode, right click to set destination.
	{
		mode = 1;
		GAMELOG("Mode: %d", mode);
		keyboard[0x31] = false;
	}
	else if (keyboard[0x32]) // 2 -> auto mode, right click will auto find path and move tank to destination.
	{
		mode = 2;
		GAMELOG("Mode: %d", mode);
		keyboard[0x32] = false;
	}
	else if (keyboard[0x33]) // 3 -> editor mode, left click will set obstacle, right click will remove obstacle.
	{
		mode = 3;
		GAMELOG("Mode: %d", mode);
		keyboard[0x33] = false;
	}

	if (keyboard[0x51]) // Q
	{
		RunAStar();		
		keyboard[0x51] = false;
	}
	else if (keyboard[0x57]) // W
	{
		DrawPath();
		keyboard[0x57] = false;
	}
	else if (keyboard[0x45]) // E
	{
		// Move player.
		_isPlayerMoving = true;		
		keyboard[0x45] = false;
	}
	else if (keyboard[0x52]) // R
	{
		ResetScene();
	}
	else if (keyboard[0x54]) // T
	{
		//obstaclesNode.clear();
		for (int x = 0; x < (WIDTH / X_STEP); x++)
		{
			for (int y = 0; y < (HEIGHT / Y_STEP); y++)
			{
				if (map[x][y]->type == Obstacle)
					map[x][y]->SetType(Empty);
			}
		}
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
	
	GivePlayerOrder();
	player->Update(dt);
}

void GridScene::OnLeftMouseDown(float x, float y)
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
		//obstaclesNode.insert(id);
		map[_x][_y]->SetType(Obstacle);
		GAMELOG("%f, %f", x, y);
		GAMELOG("(%d, %d)", _x, _y);
	}
}

void GridScene::OnRightMouseDown(float x, float y)
{
	if (!_isMouseActive) return;

	bool isInvalidDestination = false;

	switch (mode)
	{
	case 1:
		/*map[begin.GetX()][begin.GetY()]->SetType(Empty);
		begin.SetPosition(player->Position);
		begin.x = int(begin.position.x / float(X_STEP));
		begin.y = int(begin.position.y / float(Y_STEP));
		map[begin.x][begin.y]->SetType(Begin);

		if (map[int(x / float(X_STEP))][int(y / float(Y_STEP))]->type == Obstacle)
		{
			map[destination.GetX()][destination.GetY()]->SetType(Empty);
			destination.SetPosition(D3DXVECTOR2(x, y));
			destination.x = int(x / float(X_STEP));
			destination.y = int(y / float(Y_STEP));
			map[destination.x][destination.y]->SetType(Destination);
		}
		else
		{
			GAMELOG("Invalid destination!");
		}*/
		
		break;

	case 2:		
		if (!_isPlayerMoving)
		{
			ResetScene();
			/*map[begin.GetX()][begin.GetY()]->SetType(Empty);
			begin.SetPosition(player->Position);
			begin.x = int(begin.position.x / float(X_STEP));
			begin.y = int(begin.position.y / float(Y_STEP));
			map[begin.x][begin.y]->SetType(Begin);

			if (map[int(x / float(X_STEP))][int(y / float(Y_STEP))]->type == Obstacle) isInvalidDestination = true;

			if (map[destination.GetX()][destination.GetY()]->type != Obstacle)			
				map[destination.GetX()][destination.GetY()]->SetType(Empty);
						
			destination.SetPosition(D3DXVECTOR2(x, y));
			destination.x = int(x / float(X_STEP));
			destination.y = int(y / float(Y_STEP));
			map[destination.x][destination.y]->SetType(Destination);*/

			RunAStar();
			DrawPath();
			//if (isInvalidDestination) map[destination.x][destination.y]->SetType(Obstacle);
			_isPlayerMoving = true;
		}
		else
		{
			player->Stop();
			_isPlayerMoving = false;
			currentNodeIndex = 0;

			ResetScene();
			/*map[begin.GetX()][begin.GetY()]->SetType(Empty);
			begin.SetPosition(player->Position);
			begin.x = int(begin.position.x / float(X_STEP));
			begin.y = int(begin.position.y / float(Y_STEP));
			map[begin.x][begin.y]->SetType(Begin);

			if (map[int(x / float(X_STEP))][int(y / float(Y_STEP))]->type == Obstacle) isInvalidDestination = true;

			if (map[destination.GetX()][destination.GetY()]->type != Obstacle)
				map[destination.GetX()][destination.GetY()]->SetType(Empty);

			destination.SetPosition(D3DXVECTOR2(x, y));
			destination.x = int(x / float(X_STEP));
			destination.y = int(y / float(Y_STEP));
			map[destination.x][destination.y]->SetType(Destination);

			RunAStar();
			DrawPath();
			if (isInvalidDestination) map[destination.x][destination.y]->SetType(Obstacle);*/
			_isPlayerMoving = true;
		}
		break;

	case 3:
		int _x = int(x / float(X_STEP)), _y = int(y / float(Y_STEP));
		int id = _x + _y * (WIDTH / X_STEP);
		//obstaclesNode.erase(id);
		map[_x][_y]->SetType(Empty);
		GAMELOG("%f, %f", x, y);
		GAMELOG("(%d, %d)", int(x / float(X_STEP)), int(y / float(Y_STEP)));
		break;		
	}
}

void GridScene::ResetScene()
{
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			if (map[x][y]->type != Obstacle)
				map[x][y]->SetType(Empty);
			else
				map[x][y]->SetType(Obstacle);
			/*
			int id = x + y * (WIDTH / X_STEP);
			if (id % 2 == 0)
			{
				if (y % 2 == 0)
					map[x][y]->SetType(Debug0);
				else
					map[x][y]->SetType(Debug1);
			}
			else
			{
				if (y % 2 == 0)
					map[x][y]->SetType(Debug1);
				else
					map[x][y]->SetType(Debug0);
			}
			*/
		}
	}
	player->path.clear();
	drawPath.clear();
}

void GridScene::RunAStar()
{
	player->path.clear();
	Node beginNode;
	//Setting position for begin node.
	/*beginNode.SetX(begin.GetX());
	beginNode.SetY(begin.GetY());

	Node destNode;
	destNode.SetX(destination.GetX());
	destNode.SetY(destination.GetY());*/
	/*std::vector<Node> result = AStar::aStar(beginNode, destNode, map);
 	for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
	{
		player->path.push_back(map[it->GetX()][it->GetY()]);
	}	*/
}

void GridScene::DrawPath()
{
	if (!player->path.empty())
		for (int index = 0; index < player->path.size(); index++)
		{
			//path[index]->SetType(Path);
			GridTile* tile = player->path[index];
			drawPath.emplace_back(tile);
			drawPath[index]->SetType(Path);
		}
	else
		GAMELOG("Path is empty!");
}

void GridScene::GivePlayerOrder()
{
	if (_isPlayerMoving)
	{
		if (currentNodeIndex < player->path.size())
		{
			player->Move(player->path[currentNodeIndex]->position);
			if (!player->IsMoving()) currentNodeIndex++;
		}
		else
		{
			currentNodeIndex = 0;
			_isPlayerMoving = false;
		}
	}
}

void GridScene::Draw()
{
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			map[x][y]->Draw();
		}
	}
	for (int index = 0; index < drawPath.size(); index++)
	{		
		drawPath[index]->Draw();
	}
	player->Draw();
}