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
#include "StopWatch.h"

TH_ZerglingBase::TH_ZerglingBase()
{
	m_timer = new StopWatch();
}

TH_ZerglingBase::~TH_ZerglingBase()
{
	delete m_timer;
}

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
			if (SG_SITU.CurMineral() >= 150)
				e = 1;

			if (!IsGoalExist("BG_Develop"))
			{
				AddGoal("BG_Develop", new BG_Develop(rd, e));
			}
		}
	}

	Theme::Update(con);
}
