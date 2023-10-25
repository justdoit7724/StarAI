#pragma once
#include "Singleton.h"
#include <BWAPI.h>

class BigGoal;
class StopWatch;

#define SG_SITU SituationManager::Instance()
class SituationManager : public Singleton<SituationManager>
{
public:

	SituationManager();
	~SituationManager();

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

	void DisplayMinDif(Position pos);
	float GetCurMDifAvg();

	void DisplayPL(Position pos);
	void UpdatePL(bool ally, UnitType type);
	int GetPL();
	void ResetPL();

private:
	std::unordered_map<const BigGoal*, std::unordered_set<Unit>> m_regUnits;
	std::unordered_set<int> m_devUnits;

	StopWatch* m_secTimer;
	std::vector<int> m_minerals;
	std::vector<float> m_mDifs;
	std::vector<float> m_mDifAvgs;
	int m_mInterval;

	int m_plCurTotal;
	std::vector<int> m_plResourceTotals;
};

