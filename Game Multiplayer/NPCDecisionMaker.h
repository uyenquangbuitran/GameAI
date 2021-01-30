#pragma once
#include "NPC.h"

enum NPCStatus
{
	STA_Aggresive,
	STA_Normal,
	STA_Defensive
};

class NPCDecisionMaker
{
	static NPCDecisionMaker* __instance;
	NPCDecisionMaker() {}
	NPCDecisionMaker(const NPCDecisionMaker&);
	NPCDecisionMaker& operator=(const NPCDecisionMaker&);


	float _aggresivePoint = 25.f;
	float _attackRangeFactor = 1.f;
	float _attackRatioFactor = 1.f;
	float _aggresiveGainFactor = 5.f;

	int _allyCounter = 0;
	int _enemyCounter = 0;
	int _currentAllyNumber = 0;
	int _currentEnemyNumber = 0;
public:

	void GetInfo(int allyCounter, int enemyCounter)
	{
		_allyCounter = allyCounter;
		_enemyCounter = enemyCounter;
		_currentAllyNumber = allyCounter;
		_currentEnemyNumber = enemyCounter;
	}

	float GetAggresivePoint() { return _aggresivePoint; }

	void Update(bool isEnemy)
	{
		if (_allyCounter / (_currentAllyNumber - 1) >= 2)
		{
			if (10.f <= _aggresiveGainFactor)
				_aggresivePoint += _aggresiveGainFactor / 2;

			if (4.5f <= _aggresiveGainFactor && _aggresiveGainFactor <= 6.5)
				_aggresivePoint += _aggresiveGainFactor;

			if(_aggresiveGainFactor < 4.5)
				_aggresivePoint += _aggresiveGainFactor * 1.25f;
		}

		if (_enemyCounter / (_currentEnemyNumber - 1) >= 2)
		{
			if (10.f <= _aggresiveGainFactor)
				_aggresivePoint += _aggresiveGainFactor / 2;

			if (4.5f <= _aggresiveGainFactor && _aggresiveGainFactor <= 6.5)
				_aggresivePoint += _aggresiveGainFactor;

			if (_aggresiveGainFactor < 4.5)
				_aggresivePoint += _aggresiveGainFactor * 1.25f;
		}

		if (!isEnemy)
		{
			if (10.f <= _aggresiveGainFactor)
				_aggresivePoint -= 1.45f;

			if (4.5f <= _aggresiveGainFactor && _aggresiveGainFactor <= 6.5)
				_aggresivePoint -= 0.85f;

			if (_aggresiveGainFactor < 4.5)
				_aggresivePoint += 1.65f;

			_currentAllyNumber--;
		}
		else
		{
			if (10.f <= _aggresiveGainFactor)
				_aggresivePoint += 1.65;

			if (4.5f <= _aggresiveGainFactor && _aggresiveGainFactor <= 6.5)
				_aggresivePoint += 1.25f;

			if (_aggresiveGainFactor < 4.5)
				_aggresivePoint += 0.65f;

			_currentEnemyNumber--;
		}

		if (_aggresivePoint < 30.f)
		{
			SetMode(STA_Defensive);
		}
		else if (50.f < _aggresivePoint && _aggresivePoint <= 65.f)
		{
			SetMode(STA_Normal);
		}
		else if (65.f < _aggresivePoint)
		{
			SetMode(STA_Aggresive);
		}
	}

	void SetMode(NPCStatus type)
	{
		switch (type)
		{
		case STA_Aggresive:
			_aggresiveGainFactor = 10.f;
			_attackRangeFactor = 1.35f;
			_attackRatioFactor = 1.35f;
			break;
		case STA_Normal:
			_aggresiveGainFactor = 5.f;
			_attackRangeFactor = 1.f;
			_attackRatioFactor = 1.f;
			break;
		case STA_Defensive:
			_aggresiveGainFactor = 2.f;
			_attackRangeFactor = 0.7f;
			_attackRatioFactor = 1.2f;
			break;
		default:
			break;
		}
	}

	float GetAttackRangeFactor() { return _attackRangeFactor; }
	float GetAttackRatioFactor() { return _attackRatioFactor; }
	
	static NPCDecisionMaker* GetInstance()
	{
		if (__instance == nullptr) __instance = new NPCDecisionMaker();
		return __instance;
	}
};