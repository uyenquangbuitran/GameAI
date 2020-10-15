#include "GridScene.h"
#include "SceneManager.h"

void MovingTile::HandleKeyboard(std::map<int, bool> keys)
{
	if (!isAbleToMove)
	{
		color = D3DCOLOR_XRGB(128, 128, 0);
		return;
	}
	GridScene* scene = dynamic_cast<GridScene*>(SceneManager::Instance()->GetCurrentScene());
	if (keys[VK_LEFT])
	{
		position.x -= X_STEP;
		x -= 1;
		isAbleToMove = false;
	}
	else if (keys[VK_RIGHT])
	{
		position.x += X_STEP;
		x += 1;
		isAbleToMove = false;
	}
	else if (keys[VK_UP])
	{
		position.y -= X_STEP;
		y -= 1;	
		isAbleToMove = false;
	}
	else if (keys[VK_DOWN])
	{
		position.y += X_STEP;
		y += 1;
		isAbleToMove = false;
	}
	else if (keys[0x41]) // A
	{		
		scene->map[scene->begin.GetX()][scene->begin.GetY()]->SetType(Empty);
		scene->begin.SetX(x);
		scene->begin.SetY(y);
		scene->map[x][y]->SetType(Begin);
		isAbleToMove = false;
	}
	else if (keys[0x53]) // S
	{
		scene->map[scene->destination.GetX()][scene->destination.GetY()]->SetType(Empty);
		scene->destination.SetX(x);
		scene->destination.SetY(y);
		scene->map[x][y]->SetType(Destination);
		isAbleToMove = false;
	}
	else if (keys[0x44]) // D
	{
		int id = x + y * (WIDTH / X_STEP);
		obstaclesNode.insert(id);
		scene->map[x][y]->SetType(Obstacle);
		isAbleToMove = false;
	}
}

GridScene::GridScene()
{
	movingTile = new MovingTile();
	movingTile->SetPosition(D3DXVECTOR2(25.f, 25.f));

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

	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{			
			map[x][y] = new GridTile();
			map[x][y]->x = x;
			map[x][y]->y = y;
			map[x][y]->position = D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP);

			//Change value of obstacles node.
			if (obstaclesNode.count(x + y * (WIDTH / X_STEP)))
				map[x][y]->SetType(Obstacle);
		}
	}
}

void GridScene::Update(float dt)
{
	movingTile->HandleKeyboard(keyboard);
	if (keyboard[0x46]) // F
	{
		movingTile->isAbleToMove = true;
		movingTile->color = D3DCOLOR_XRGB(255, 255, 0);
	}
	else if (keyboard[0x52]) // R
	{
		for (int x = 0; x < (WIDTH / X_STEP); x++)
		{
			for (int y = 0; y < (HEIGHT / Y_STEP); y++)
			{
				map[x][y]->SetType(Empty);
			}
		}
		obstaclesNode.clear();
		path.clear();
	}
	else if (keyboard[0x45]) // E
	{
		path.clear();
		Node beginNode;
		//Setting position for begin node.
		beginNode.SetX(begin.GetX());
		beginNode.SetY(begin.GetY());

		Node destNode;
		destNode.SetX(destination.GetX());
		destNode.SetY(destination.GetY());
		std::vector<Node> result = AStar::aStar(beginNode, destNode, map);
		for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
		{
			path.push_back(map[it->GetX()][it->GetY()]);
		}
	}
	else if (keyboard[0x51])
	{
		if (!path.empty())
			for (int index = 0; index < path.size(); index++)
			{
				path[index]->SetType(Path);
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

	movingTile->Draw();
}