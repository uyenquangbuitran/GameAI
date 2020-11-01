#include "GridTile.h"
#include "SpriteList.h"

GridTile::GridTile()
{
	anim = new Animation();
	anim->addFrameInfo(FrameInfo(SpriteList::Instance()->WhiteTile, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
	type = Empty;
	color = D3DCOLOR_XRGB(0, 0, 0);
}

GridTile::GridTile(const GridTile & tile)
{
	x = tile.x;
	y = tile.y;
	anim = new Animation();
	anim->addFrameInfo(FrameInfo(SpriteList::Instance()->WhiteTile, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
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
		color = D3DCOLOR_XRGB(0, 0, 0); // black
		break;
	case Opened:
		color = D3DCOLOR_XRGB(255, 255, 255); // white
		break;
	case Closed:
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