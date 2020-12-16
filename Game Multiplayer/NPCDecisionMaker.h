#pragma once
#include "NPC.h"

enum NPCStatus
{
	Aggresive,
	Defensive
};

class NPCDecisionMaker
{
	static NPCDecisionMaker* __instance;

public:		

	NPCDecisionMaker* GetInstance()
	{
		if (__instance == nullptr) __instance = new NPCDecisionMaker();

		return __instance;
	}
};