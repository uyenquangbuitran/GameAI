#include "GameMap.h"
#include "GameLog.h"
#include "AStar.h"

GameMap::GameMap(char* filePath)
{
	LoadMap(filePath);
	//debugDraw = new GameDebugDraw();
}

void GameMap::LoadMap(char* filePath)
{
	_map = new Tmx::Map();
	_map->ParseFile(filePath);

	// đưa danh sách hình ảnh tileset vào tilesetList
	for (int i = 0; i < _map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = _map->GetTileset(i);
		//char* link = "Resource files/%s", tileset->GetImage()->GetSource().c_str();
		Sprite *sprite = new Sprite("Resource files/tileset.png", D3DXCOLOR(255, 255, 255, 255));
		_tilesetList.insert(std::pair<int, Sprite*>(i, sprite));
	}

	_waterList.resize(64);
	_brickList.resize(64);
	_brickNorList.resize(64);

	// duyệt các layer của map
	for (int i = 0; i < _map->GetNumTileLayers(); i++)
	{	
		const Tmx::TileLayer *layer = _map->GetTileLayer(i);
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Tile Layer 1") 
		{
			/*for (int j = 0; j < _map->GetNumTilesets(); j++)
			{
				const Tmx::Tileset *tileSet = _map->GetTileset(j);*/

				int tileWidth = _map->GetTileWidth();
				int tileHeight = _map->GetTileHeight();

				for (int m = 0; m < layer->GetHeight(); m++)
				{
					for (int n = 0; n < layer->GetWidth(); n++)
					{
						if (layer->GetTileTilesetIndex(n, m) != -1)
						{
							// tọa độ object
							D3DXVECTOR2 pos((float)(n * tileWidth + tileWidth / 2), 
								(float)(m * tileHeight + tileHeight / 2));

							int listId = n / 15 + m / 15 * MAX_LEVEL2_TILE;

							// khởi tạo brick
							Brick* brick;
							if (layer->GetName() == "Brick")
							{
								brick = new BrickNormal(pos);
								_brickNorList[listId].push_back((BrickNormal*)brick);

								int _x = int(pos.x / float(X_STEP)), _y = int(pos.y / float(Y_STEP));
								gridMapLv3[_x][_y] = new MapTileLv3(_x, _y);
								gridMapLv3[_x][_y]->SetType(NormalBrick);
								gridMapLv3[_x][_y]->SetCost(NormalBrick);
								/*int id = _x + _y * (WIDTH / X_STEP);								
								obstaclesNodes.insert(id);*/
							}
							else if (layer->GetName() == "Metal Brick")
							{
								brick = new MetalBrick(pos);

								int _x = int(pos.x / float(X_STEP)), _y = int(pos.y / float(Y_STEP));
								gridMapLv3[_x][_y] = new MapTileLv3(_x, _y);
								gridMapLv3[_x][_y]->SetType(Metal);
								gridMapLv3[_x][_y]->SetCost(Metal);
								int id = _x + _y * (MAP_WIDTH / X_STEP);								
								obstaclesNodes.insert(id);
							}
							else if (layer->GetName() == "Water")
							{
								brick = new Water(pos);
								_waterList[listId].push_back((Water*)brick);

								int _x = int(pos.x / float(X_STEP)), _y = int(pos.y / float(Y_STEP));
								gridMapLv3[_x][_y] = new MapTileLv3(_x, _y);
								gridMapLv3[_x][_y]->SetType(WaterBrick);
								gridMapLv3[_x][_y]->SetCost(WaterBrick);
								int id = _x + _y * (MAP_WIDTH / X_STEP);
								obstaclesNodes.insert(id);
							}
							else if (layer->GetName() == "Tile Layer 1")
							{
								brick = new Boundary(pos);

								int _x = int(pos.x / float(X_STEP)), _y = int(pos.y / float(Y_STEP));
								gridMapLv3[_x][_y] = new MapTileLv3(_x, _y);
								gridMapLv3[_x][_y]->SetType(Border);
								gridMapLv3[_x][_y]->SetCost(Border);
								int id = _x + _y * (MAP_WIDTH / X_STEP);
								obstaclesNodes.insert(id);
							}

							_brickList[listId].push_back(brick);
						}
					}
				}
			/*}*/
		}
	}
	//end load layer.

	//Load grid lv3.
	const Tmx::ObjectGroup* group = _map->GetObjectGroup(0);
	vector<Tmx::Object*> objects = group->GetObjects();

	//Load gate tile.
	/*for (int index = 0; index < objects.size(); index++)
	{
		int x = objects[index]->GetX() / X_STEP;
		int y = objects[index]->GetY() / Y_STEP;
		if (gridMapLv3[x][y] == nullptr)
		{
			gridMapLv3[x][y] = new MapTileLv3(x, y);
			gridMapLv3[x][y]->SetType(Gate);
			gridMapLv3[x][y]->SetCost(Gate);
		}
		else if (objects[index]->GetName() == "BG") { gridMapLv3[x][y]->SetType(BrickGate); }
		
		gridMapLv3[x][y]->SetGateInfo(objects[index]->GetType());
		if (gridMapLv3[x][y]->GetType() == Empty) gridMapLv3[x][y]->SetCost(Empty);
	}*/
	//end load gate.

	for (int x = 0; x < MAP_WIDTH / X_STEP; x++)
	{
		for (int y = 0; y < MAP_HEIGHT / Y_STEP; y++)
		{
			if (gridMapLv3[x][y] == nullptr)
			{
				gridMapLv3[x][y] = dynamic_cast<MapTileLv3*>(new MapTileLv3(x, y));
				gridMapLv3[x][y]->SetCost(Empty);
			}
		}
	}
	//end load grid lv3.

	//Load grid lv2.
	objects.clear();
	group = _map->GetObjectGroup(1);
	objects = group->GetObjects();
	for (int index = 0; index < objects.size(); index++)
	{
		int x = objects[index]->GetX() * 8 / MAP_WIDTH;
		int y = objects[index]->GetY() * 8 / MAP_HEIGHT;
		gridMapLv2[x][y] = new MapTileLv2(x, y);		
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				gridMapLv2[x][y]->AddChildTile(gridMapLv3[i][j]);
				/*if (gridMapLv3[i + x * 15][j + y * 15]->GetType() == Gate)
					gridMapLv2[x][y]->AddGateTile(gridMapLv3[i + x * 15][j + y * 15]);*/
			}
		}

		/*if (objects[index]->GetType() != "")
		{
			gridMapLv2[x][y]->SetType(Gate);
			gridMapLv2[x][y]->SetGateInfo(objects[index]->GetType());
		}*/
	}

	/*for (int i = 0; i < 15; i++)
	{
		GAMELOG("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			gridMapLv3[0][i]->GetType(),
			gridMapLv3[1][i]->GetType(), gridMapLv3[2][i]->GetType(), gridMapLv3[3][i]->GetType(),
			gridMapLv3[4][i]->GetType(), gridMapLv3[5][i]->GetType(), gridMapLv3[6][i]->GetType(),
			gridMapLv3[7][i]->GetType(), gridMapLv3[8][i]->GetType(), gridMapLv3[9][i]->GetType(),
			gridMapLv3[10][i]->GetType(), gridMapLv3[11][i]->GetType(), gridMapLv3[12][i]->GetType(),
			gridMapLv3[13][i]->GetType(), gridMapLv3[14][i]->GetType(), gridMapLv3[15][i]->GetType(),
			gridMapLv3[16][i]->GetType(), gridMapLv3[17][i]->GetType(), gridMapLv3[18][i]->GetType(),
			gridMapLv3[19][i]->GetType(), gridMapLv3[20][i]->GetType(), gridMapLv3[21][i]->GetType(),
			gridMapLv3[22][i]->GetType(), gridMapLv3[23][i]->GetType(), gridMapLv3[24][i]->GetType(),
			gridMapLv3[25][i]->GetType(), gridMapLv3[26][i]->GetType(), gridMapLv3[27][i]->GetType(),
			gridMapLv3[28][i]->GetType(), gridMapLv3[29][i]->GetType());
	}*/

	//Load grid lv1.	
	objects.clear();
	group = _map->GetObjectGroup(2);
	objects = group->GetObjects();
	for (int index = 0; index < objects.size(); index++)
	{
		int x = objects[index]->GetX() * 2 / MAP_WIDTH;
		int y = objects[index]->GetY() * 2 / MAP_HEIGHT;
		gridMapLv1[x][y] = new MapTileLv1(x, y);		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				gridMapLv1[x][y]->AddChildTile(gridMapLv2[i][j]);
				/*if (gridMapLv2[i + x * 4][j + y * 4]->GetType() == Gate)
					gridMapLv1[x][y]->AddGateTile(gridMapLv2[i + x * 4][j + y * 4]);*/
			}
		}
	}	
}

void GameMap::Draw()
{
	// vẽ các object viên brick
	for (int i = 0; i < (int)_brickList.size(); i++)
	{
		/*_brickList[i]->Draw();*/
	}

	// vẽ các hình ảnh khác lên
	for (int i = 0; i < _map->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = _map->GetTileLayer(i);

		 //layer là các viên brick đã khởi tạo => bỏ qua
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Tile Layer 1")
			continue;

		if (!layer->IsVisible())
		{
			continue;
		}

		RECT sourceRECT;

		int tileWidth = _map->GetTileWidth();
		int tileHeight = _map->GetTileHeight();

		for (int m = 0; m < layer->GetHeight(); m++)
		{
			for (int n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = _map->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = _tilesetList[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);
					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					// xác định hcn trong hình ảnh gốc
					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					// vị trí để vẽ map lên
					D3DXVECTOR2 pos(n * tileWidth + tileWidth / 2.f, 
						m * tileHeight + tileHeight / 2.f);

					// vẽ lên
					sprite->setPosition(pos);
					sprite->setRect(sourceRECT);
					sprite->setCenter(D3DXVECTOR2(tileWidth / 2.f, tileHeight / 2.f));
					sprite->Draw();
				}
			}

		}
	}
}

void GameMap::Draw(Camera _camera)
{
	D3DXVECTOR2 offset(GameGlobal::Width / 2.f - _camera.Position.x,
		GameGlobal::Height / 2.f - _camera.Position.y);

	// vẽ các object viên brick
	for (int i = 0; i < (int)_brickList.size(); i++)
	{
		for (int j = 0; j < (int)_brickList[i].size(); j++)
			_brickList[i][j]->Draw(offset);
	}

	// vẽ các hình ảnh khác lên
	for (int i = 0; i < _map->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = _map->GetTileLayer(i);

		//layer là các viên brick đã khởi tạo => bỏ qua
		if (layer->GetName() == "Brick" ||
			layer->GetName() == "Metal Brick" ||
			layer->GetName() == "Water" ||
			layer->GetName() == "Tile Layer 1")
			continue;

		if (!layer->IsVisible())
		{
			continue;
		}

		int tileWidth = _map->GetTileWidth();
		int tileHeight = _map->GetTileHeight();

		for (int m = 0; m < layer->GetHeight(); m++)
		{
			for (int n = 0; n < layer->GetWidth(); n++)
			{
				// vị trí để vẽ map lên
				D3DXVECTOR2 pos(n * tileWidth + tileWidth / 2.f,
					m * tileHeight + tileHeight / 2.f);



				pos += offset;

				// check outside camera
				RECT cameraRect = _camera.getBound();
				if (!(pos.x - tileWidth / 2.f <= cameraRect.right &&
					pos.x + tileWidth / 2.f >= cameraRect.left &&
					pos.y - tileHeight / 2.f <= cameraRect.bottom &&
					pos.y + tileHeight / 2.f >= cameraRect.top))
				{
					continue;
				}

				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = _map->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = _tilesetList[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);
					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					// xác định hcn trong hình ảnh gốc
					RECT sourceRECT;
					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					// vẽ lên
					sprite->setPosition(pos);
					sprite->setRect(sourceRECT);
					sprite->setCenter(D3DXVECTOR2(tileWidth / 2.f, tileHeight / 2.f));
					sprite->Draw();
				}
			}

		}
	}
}