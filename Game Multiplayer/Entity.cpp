#include "Entity.h"
#include "NPCDecisionMaker.h"

RECT Entity::getBound()
{
	RECT bound;

	bound.left = (long)(Position.x - _width / 2.f);
	bound.right = (long)(Position.x + _width / 2.f);
	bound.top = (long)(Position.y - _height / 2.f);
	bound.bottom = (long)(Position.y + _height / 2.f);

	return bound;
}

void Entity::Damage(int value)
{
	_hp -= value;
	if (_hp <= 0)
	{
		IsDeleted = true;
		if (_type == ET_Player) NPCDecisionMaker::GetInstance()->Update(true);
		else if (_type == ET_NPC) NPCDecisionMaker::GetInstance()->Update(false);
	}
}