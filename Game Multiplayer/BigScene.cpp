#include "BigScene.h"
#include "AStar.h"

BigScene::BigScene()
{
	//Initilize map.
	_map = new GameMap("Resource files/ai_map5.tmx");
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			gridMap[x][y] = new GridTile();
			gridMap[x][y]->SetVec(x, y);
			gridMap[x][y]->position = D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP);
		}
	}

	//Initilize camera.
	camera.Init(GameGlobal::Height, GameGlobal::Width, 0, 10000, 0, 10000);
	camera.setPosition(D3DXVECTOR2(0.f, 0.f));
	cameraOffset = D3DXVECTOR2(GameGlobal::Width / 2.f - camera.Position.x,
		GameGlobal::Height / 2.f - camera.Position.y);

	//Initilize player.
	_player = new Player();
	_player->Position = D3DXVECTOR2(48.f, 48.f);

	//Initilize player units.


	//Initilize NPC units.


	//Inittilize bullets.
	/*for (int counter = 0; counter < _npcList.size() + 1; counter++)
	{
		Bullet* bullet = new Bullet();
		_bulletList.emplace_back(bullet);
	}*/

	
}

void BigScene::Update(float dt)
{
#pragma region GameModeInput
	if (keyboard[0x31]) // 1 -> .
	{
		
		keyboard[0x31] = false;
	}
	else if (keyboard[0x32]) // 2 -> .
	{
		
		keyboard[0x32] = false;
	}
	else if (keyboard[0x33]) // 3 -> .
	{
		
		keyboard[0x33] = false;
	}
#pragma endregion

#pragma region KeyboardCommandInput
	if (keyboard[0x51]) // Q
	{
		
		keyboard[0x51] = false;
	}
	else if (keyboard[0x57]) // W
	{
		
		keyboard[0x57] = false;
	}
	else if (keyboard[0x45]) // E
	{
		
		keyboard[0x45] = false;
	}
	else if (keyboard[0x52]) // R
	{
		
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
#pragma endregion
		
#pragma region CameraCommand
	if (keyboard[0x25]) // Left
	{
		camera.Position.x -= 32.f;
		keyboard[0x25] = false;
	}
	else if (keyboard[0x26]) // Up
	{
		camera.Position.y -= 32.f;
		keyboard[0x26] = false;
	}
	else if (keyboard[0x27]) // Right
	{
		camera.Position.x += 32.f;
		keyboard[0x27] = false;
	}
	else if (keyboard[0x28]) // Down
	{
		camera.Position.y += 32.f;
		keyboard[0x28] = false;
	}
#pragma endregion

#pragma region UpdateObjects
	for (auto bullet : _bulletList) bullet->Update(dt);	

	for (auto npc : _npcList)
	{
		npc->Update(dt);
	}

	_player->Update(dt);
#pragma endregion

}

void BigScene::OnLeftMouseDown(float x, float y)
{
	if (!_isMouseActive) return;
	GAMELOG("(%d, %d)", int(x / float(X_STEP)), int(y / float(Y_STEP)));
}

void BigScene::OnRightMouseDown(float x, float y)
{
	//Adjust x and y coordinate with offset when camera moves.
	x -= cameraOffset.x;
	y -= cameraOffset.y;
	if (!_isMouseActive) return;
	Node begin, destination;
	bool isInvalidDestination = false;
	if (!_player->hasOrder)
	{

		_player->path.clear();
		/*if (gridMap[begin.GetX()][begin.GetY()]->type != Obstacle)
			gridMap[begin.GetX()][begin.GetY()]->SetType(Empty);*/
		
		//begin.SetPosition(_player->Position);
		begin.SetXY(int(_player->Position.x / float(X_STEP)), int(_player->Position.y / float(Y_STEP)));
		//gridMap[begin.x][begin.y]->SetType(Begin);

		isInvalidDestination = IsValidClickPosition(int(x / float(X_STEP)), int(y / float(Y_STEP)));

		/*if (gridMap[destination.GetX()][destination.GetY()]->type != Obstacle)
			gridMap[destination.GetX()][destination.GetY()]->SetType(Empty);*/

		//destination.SetPosition(D3DXVECTOR2(x, y));
		destination.SetXY(int(x / float(X_STEP)), int(y / float(Y_STEP)));
		//*destinationNode = destination;
		//gridMap[destination.x][destination.y]->SetType(Destination);

		//UpdateTankNodes();
		FindPlayerPath(begin, destination);
		//DrawPath();
		//if (isInvalidDestination) gridMap[destination.x][destination.y]->SetType(Obstacle);
		_player->hasOrder = true;
	}
	else
	{
		_player->Stop();
		_player->hasOrder = false;
		_player->currentNodeIndex = 0;

		_player->path.clear();

		/*if (gridMap[begin.GetX()][begin.GetY()]->type != Obstacle)
			gridMap[begin.GetX()][begin.GetY()]->SetType(Empty);*/

		//begin.SetPosition(_player->Position);
		begin.SetXY(int(_player->Position.x / float(X_STEP)), int(_player->Position.y / float(Y_STEP)));
		//gridMap[begin.x][begin.y]->SetType(Begin);

		isInvalidDestination = IsValidClickPosition(int(x / float(X_STEP)), int(y / float(Y_STEP)));

		/*if (gridMap[destination.GetX()][destination.GetY()]->type != Obstacle)
			gridMap[destination.GetX()][destination.GetY()]->SetType(Empty);*/

		//destination.SetPosition(D3DXVECTOR2(x, y));
		destination.SetXY(int(x / float(X_STEP)), int(y / float(Y_STEP)));
		//*destinationNode = destination;
		//gridMap[destination.x][destination.y]->SetType(Destination);

		//UpdateTankNodes();
		FindPlayerPath(begin, destination);
		//DrawPath();
		//if (isInvalidDestination) gridMap[destination.x][destination.y]->SetType(Obstacle);
		_player->hasOrder = true;
	}
}

void BigScene::UpdateTankNodes()
{
	tankNodes.clear();
	int _x = int(_player->Position.x / float(X_STEP)), _y = int(_player->Position.y / float(Y_STEP));
	int id = _x + _y * (WIDTH / X_STEP);
	tankNodes.insert(id);
	for (auto npc : _npcList)
	{
		_x = int(npc->Position.x / float(X_STEP));
		_y = int(npc->Position.y / float(Y_STEP));
		id = _x + _y * (WIDTH / X_STEP);
		tankNodes.insert(id);
	}
}

void BigScene::FindPlayerPath(Node begin, Node destination)
{
	_player->path.clear();

	std::vector<Node> result = AStar::GetInstance()->aStar(begin, destination, PART_SIZE);

	for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
	{
		_player->path.emplace_back(gridMap[it->GetX()][it->GetY()]);
	}	
}

void BigScene::Draw()
{
	_map->Draw(camera);
	_player->Draw(cameraOffset);
}