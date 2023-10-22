#pragma once
#include "Singleton.h"
#include <BWAPI.h>

class BigGoal;

#define SG_SITU SituationManager::Instance()
class SituationManager : public Singleton<SituationManager>
{
public:

	void Update();

	bool IsExist(bool isAlly, BWAPI::UnitType type);
	int OpenMinerals(Unit resourceDepot);
	bool GetOpenPositionNear(Position pos, Position& outPos);
	Unit GetGasNear(Position pos);
	bool IsBuildable(TilePosition pos, int w, int h);

	void RegisterUnit(const BigGoal* goal, Unit unit);
	void UnregisterUnit(Unit unit);
	void UnregisterGoal(const BigGoal* goal);
	bool IsUnitRegistered(Unit unit);
	std::unordered_set<Unit> GetRegistered(const BigGoal* goal);
	std::vector<Unit> UnitsInRange(bool isAlly, TilePosition center, UnitType type, int range=100000);
	std::vector<Unit> UnitsInRange(bool isAlly, Unit centerUnit, UnitType type, int range = 100000);
	std::vector<Unit> AllUnitsinRange(bool isAlly, TilePosition center, int range=100000);

	TilePosition GetCenterTPos(Unit unit);

	int CurMineral();
	int CurGas();
	int GetValidSupply();

	void AddDevUnit(UnitType type);
	void RemoveDevUnit(UnitType type);
	bool IsDeveloping(UnitType type);

	void GetUnitPrice(UnitType type, int& mineral, int& gas);

private:
	std::unordered_map<const BigGoal*, std::unordered_set<Unit>> m_regUnits;
	std::unordered_set<int> m_devUnits;


	
};

