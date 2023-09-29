#include "pch.h"
#include "TH_ZerglingBase.h"
#include "SituationManager.h"
#include "BG_Develop.h"
#include "BG_Attack.h"
#include "MapManager.h"
#include "TechTree.h"
#include "DebugManager.h"
#include "LogManager.h"
#include "AStar.h"

void TH_ZerglingBase::Update(const Controller* con)
{
	auto rd = SG_SITU.UnitsInRange(true,TilePosition(32,32),  UnitTypes::Zerg_Hatchery)[0];

	if (rd)
	{
		auto zerglings = SG_SITU.UnitsInRange(true, rd->getTilePosition(), UnitTypes::Zerg_Zergling, 10000);
		int zerglingCount = 0;
		for (int i = 0; i < zerglings.size(); ++i)
		{
			if (!SG_SITU.IsUnitRegistered(zerglings[i]))
				zerglingCount++;
		}

		if (SG_TECH.IsValid(UnitTypes::Zerg_Zergling) && zerglingCount >= 6)
		{
			auto pts = SG_MAP.GetPts();

			AddGoal("BG_Attack", new BG_Attack(pts[0]));
		}
		else
		{


			float e = 0;
			int minerals = Broodwar->self()->gatheredMinerals();
			if (minerals >= 150)
				e = 1;

			if (!IsGoalExist("BG_Develop"))
			{
				AddGoal("BG_Develop", new BG_Develop(rd, e));
			}
		}
	}

	std::vector<std::string> delList;
	for (auto it = m_goals.begin(); it != m_goals.end();it++)
	{
		for (int i=0; i<it->second.size();)
		{
			it->second[i]->Update(con);
#ifdef _DEBUG
			it->second[i]->Debug();
#endif

			if (it->second[i]->Finished())
			{
				delete it->second[i];
				it->second.erase(it->second.begin() + i);
			}
			else
				i++;
		}

		if (it->second.empty())
			delList.push_back(it->first);
	}

	for (int i = 0; i < delList.size();  ++i)
	{
		m_goals.erase(delList[i]);
	}
}
