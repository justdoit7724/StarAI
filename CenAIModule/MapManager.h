#pragma once
#include "SwapChain.h"
#include "Singleton.h"
#include "AStar.h"

#define SG_MAP MapManager::Instance()
class MapManager : public Singleton<MapManager>
{
public:

	MapManager();
	~MapManager();

	int GetWidth();
	int GetHeight();
	int GetWeightWidth();
	int GetWeightHeight();
	int GetTerrain(WalkPosition pos);
	float GetAtt(TilePosition pos);
	float GetDef(TilePosition pos);
	float GetTotalWeight(TilePosition pos);
	std::vector<TilePosition> GetPts() const;
	bool IsValidPos(TilePosition tpos);
	bool IsValidPos(Position pos);

	void Update();

	void DisplayAtt();
	void DisplayDef();
	void DisplayTerrain();
	void DisplayTotal();
	void DisplayPts();
	void DisplayBuildableCheck();
	void TurnOffDisplay();

	Position GetExpPt();
	bool IsExpOccupied();
	bool GetAddExpPt(Position& pos);


	bool isDisplayAtt;
	bool isDisplayDef;
	bool isDisplayTerrain;
	bool isDisplayTotal;
	bool isDisplayPts;

private:

	typedef std::vector<std::vector<int>> basicMap;
	typedef std::vector<std::vector<float>> weighMap;
	typedef std::vector<std::pair<int,int>> weighPts;
	//walk pos based
	std::unique_ptr<SwapChain<basicMap>> m_terrain;
	std::vector<Unit> m_tmpUnits;

	//tile pos based
	std::unique_ptr<SwapChain<weighMap>> m_wmAtt;
	std::unique_ptr<SwapChain<weighMap>> m_wmDef;
	std::unique_ptr<SwapChain<weighMap>> m_wmTotal;

	std::unique_ptr<SwapChain<weighPts>> m_wPts;
	std::map<float, int> m_sorter;
	int m_Width;
	int m_height;
	int m_WeightWidth;
	int m_WeightHeight;

	//sorting related
	std::unordered_set<int> m_highests;
	
	std::vector<void*> m_enemies;

	//not used for now
	std::unique_ptr<SwapChain<basicMap>> m_state;

	//expansion
	bool m_isFoundExpansion;
	AStar::Generator* m_generator;
	Unit m_myRD;
	Unit m_emRD;
	AStar::CoordinateList m_path;
	std::vector<Unit> m_expMinerals;
	Unit m_expGas;
	std::vector<Position> m_expPts;
	Position m_expPt;

};

