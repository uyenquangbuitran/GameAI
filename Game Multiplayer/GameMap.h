#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include <set>

#include "Sprite.h"	
#include "MapReader\Tmx.h.in"
#include "GameGlobal.h"
#include "Brick.h"
#include "BrickNormal.h"
#include "MetalBrick.h"
#include "Water.h"
#include "Boundary.h"
#include "GameCollision.h"
#include "GameDebugDraw.h"
#include "QuadTree.h"
#include "Camera.h"

class GameMap
{
	std::vector<Brick*> _brickList;
	std::vector<BrickNormal*> _brickNorList;
	Tmx::Map *_map;
	std::map<int, Sprite*>  _tilesetList;
	GameDebugDraw* _debugDraw;

	void LoadMap(char* filePath);
	int getWidth() { return _map->GetWidth() * _map->GetTileWidth(); }
	int getHeight() { return _map->GetHeight() * _map->GetTileHeight(); }
	int getTileWidth() { return _map->GetTileWidth(); }
	int getTileHeight() { return _map->GetTileHeight(); }

public:
	std::set<int, std::greater <int>> obstaclesNode;

	GameMap(char* filePath);
	~GameMap() {}
	void Draw();
	Tmx::Map* getMap() { return _map; }
	std::vector<Brick*> getBrickList() { return _brickList; }
	std::vector<BrickNormal*> getBrickNorList() { return _brickNorList; }

	void Draw(Camera _camera)
	{
		// vẽ các object viên brick
		for (int i = 0; i < (int)_brickList.size(); i++)
		{
			_brickList[i]->Draw();
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

					D3DXVECTOR2 offset(GameGlobal::Width / 2.f - _camera.Position.x,
						GameGlobal::Height / 2.f - _camera.Position.y);

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
};

