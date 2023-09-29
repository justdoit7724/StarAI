#pragma once
#include "Singleton.h"
#include <BWAPI.h>

class BigGoal;

#define SG_SITU SituationManager::Instance()
class SituationManager : public Singleton<SituationManager>
{
public:
	bool IsExist(bool isAlly, BWAPI::UnitType type);
	int OpenMinerals(Unit resourceDepot);
	bool GetOpenPositionNear(TilePosition pos, TilePosition& outPos);
	bool IsBuildable(TilePosition ltPos, int w, int h);

	void RegisterUnit(const BigGoal* goal, Unit unit);
	void UnregisterUnit(Unit unit);
	void UnregisterGoal(const BigGoal* goal);
	bool IsUnitRegistered(Unit unit);
	std::unordered_set<Unit> GetRegistered(const BigGoal* goal);
	std::vector<Unit> UnitsInRange(bool isAlly, TilePosition center, UnitType type, int range=100000);
	std::vector<Unit> UnitsInRange(bool isAlly, Unit centerUnit, UnitType type, int range = 100000);
	std::vector<Unit> AllUnitsinRange(bool isAlly, TilePosition center, int range=100000);

	TilePosition GetCenterTPos(Unit unit);

private:
	std::unordered_map<const BigGoal*, std::unordered_set<Unit>> m_regUnits;
};

