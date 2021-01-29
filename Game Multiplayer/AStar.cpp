#include "AStar.h"

AStar* AStar::_instance = nullptr;

AStar* AStar::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new AStar();
		_instance->Init();
	}

	return _instance;		
}

void AStar::Init()
{
	for (int x = 0; x < (MAP_WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (MAP_HEIGHT / Y_STEP); y++)
		{			
			map[x][y] = 0;
		}
	}
}
