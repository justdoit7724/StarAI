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

	void Init(Unit base);
	Unit GetBase();

	void Update();

	bool IsExist(bool isAlly, BWAPI::UnitType type);
	int OpenMinerals(Unit resourceDepot);
	
	bool GetOpenPositionNear(Position pos, Position& outPos, int w, int h, double dist=-1);
	Unit GetGasNear(Position pos, int range=400);
	std::vector<Unit> GetMineralsNear(Position pos, int range = 300);
	bool IsBuildable(TilePosition pos, int w, int h, bool isNeedCreep=true);

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

	void AddDevUnit(Unit center, UnitType u, int count=1);
	void RemoveDevUnit(Unit center, UnitType u, int count=1);
	int IsDeveloping(UnitType type,Unit center=nullptr);

	void GetUnitPrice(UnitType type, int& mineral, int& gas);

	void DisplayMinDif(Position pos);
	float GetCurMDifAvg();

	void DisplayPL(Position pos);
	void UpdatePL(bool ally, UnitType type);
	int GetPL();
	void ResetPL();

private:
	Unit m_base;

	std::unordered_map<const BigGoal*, std::unordered_set<Unit>> m_regUnits;
	std::unordered_map<Unit, std::unordered_map<int, int>> m_devUnits;

	StopWatch* m_secTimer;
	std::vector<int> m_minerals;
	std::vector<float> m_mDifs;
	std::vector<float> m_mDifAvgs;
	int m_mInterval;

	int m_plCurTotal;
	std::vector<int> m_plResourceTotals;
};

