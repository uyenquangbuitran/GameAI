#include "GridTile.h"
#include "SpriteList.h"

GridTile::GridTile()
{
	anim = new Animation();
	anim->addFrameInfo(FrameInfo(SpriteList::Instance()->WhiteTile, 0, 32, 0, 32, D3DXVECTOR2(16.f, 16.f)));
	type = Empty;
	color = D3DCOLOR_XRGB(0, 0, 0);
}

GridTile::GridTile(const GridTile & tile)
{
	x = tile.x;
	y = tile.y;
	anim = new Animation();
	anim->addFrameInfo(FrameInfo(SpriteList::Instance()->WhiteTile, 0, 32, 0, 32, D3DXVECTOR2(16.f, 16.f)));
	position = tile.position;
	SetType(tile.type);
}

void GridTile::Draw()
{
	anim->Draw(position, D3DXVECTOR2(0.f, 0.f), false, false, color);
}

void GridTile::SetType(TileType type)
{
	this->type = type;
	switch (type)
	{
	case Empty:
		if (x == 0 && y == 0)
		{
			return;
		}
		color = D3DCOLOR_XRGB(0, 0, 0); // black
		break;
	case Opened:
		if (x == 0 && y == 0)
		{
			return;
		}
		color = D3DCOLOR_XRGB(255, 255, 255); // white
		break;
	case Closed:
		if (x == 0 && y == 0)
		{
			return;
		}
		color = D3DCOLOR_XRGB(128, 128, 128); // grey
		break;
	case Obstacle:
		color = D3DCOLOR_XRGB(255, 0, 255); // purple
		break;
	case Moving:
		color = D3DCOLOR_XRGB(255, 255, 0); // yellow
		break;
	case Begin:
		color = D3DCOLOR_XRGB(0, 0, 255); // blue
		break;
	case Destination:
		color = D3DCOLOR_XRGB(255, 0, 0); // red
		break;
	case Path:
		color = D3DCOLOR_XRGB(0, 255, 0); // green
		break;
	case Debug0:
		color = D3DCOLOR_XRGB(0, 128, 0); // green
		break;
	case Debug1:
		color = D3DCOLOR_XRGB(128, 128, 0); // yellow
	}
}