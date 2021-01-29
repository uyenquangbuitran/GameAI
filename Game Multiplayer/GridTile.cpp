#include "GridTile.h"
#include "SpriteList.h"

GridTile::GridTile()
{
	anim = new Animation();
	anim->addFrameInfo(FrameInfo(SpriteList::Instance()->WhiteTile, 0, 32, 0, 32, D3DXVECTOR2(16.f, 16.f)));
	type = Empty;
	color = D3DCOLOR_XRGB(0, 0, 0);
}

void GridTile::Draw()
{
	anim->Draw(position, D3DXVECTOR2(0.f, 0.f), false, false, color);
}

void GridTile::Draw(D3DXVECTOR2 offset)
{
	anim->Draw(position, offset, false, false, color);
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