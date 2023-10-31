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
#include "BG_Expansion.h"

TH_ZerglingBase::TH_ZerglingBase()
	:Theme(ID())
{
	m_timer = new StopWatch();
	m_timer2 = new StopWatch();
	m_zerglingCount = 6;
	m_plCompensate = 0;
}

TH_ZerglingBase::~TH_ZerglingBase()
{
	delete m_timer;
	delete m_timer2;
}

void TH_ZerglingBase::Update(const Controller* con)
{
	if (m_timer2->elapsed() > 5)
	{
		m_timer2->reset();

		auto mPL = SG_SITU.GetPL() + m_plCompensate;
		if (mPL < -150)
		{
			m_zerglingCount += 2;
			m_plCompensate += 150;
		}
	}

	if (m_timer->elapsed() > 0.5)
	{
		m_timer->reset();

		auto rd = SG_SITU.UnitsInRange(true, TilePosition(32, 32), UnitTypes::Zerg_Hatchery);
		if (rd.size())
		{
			std::vector<UnitType> units;
			std::vector<int> unitCounts;
			std::vector<UnitType> trainUnits;
			std::vector<int> trainUnitCounts;
			units.push_back(UnitTypes::Zerg_Zergling);
			unitCounts.push_back(m_zerglingCount);

			bool isTroopReady = true;
			for (int i = 0; i < units.size(); ++i)
			{
				auto curUnits = SG_SITU.UnitsInRange(true, rd[0]->getTilePosition(), units[i], 10000);
				int curUnitCount = 0;
				for (int i = 0; i < curUnits.size(); ++i)
				{
					if (!SG_SITU.IsUnitRegistered(curUnits[i]))
						curUnitCount++;
				}
				if (curUnitCount < unitCounts[i])
				{
					isTroopReady = false;

					trainUnits.push_back(units[i]);
					trainUnitCounts.push_back(unitCounts[i]-curUnitCount);
				}
			}

			if (isTroopReady)
			{
				auto pts = SG_MAP.GetPts();

				AddGoal(new BG_Attack(Position(pts[0]), units,unitCounts));
			}

			AddGoal(new BG_Develop(rd[0], trainUnits, trainUnitCounts, 0));


			if (SG_SITU.GetCurMDifAvg() > 15 && SG_SITU.CurMineral() > 250 && !SG_SITU.IsDeveloping(UnitTypes::Zerg_Hatchery))
			{
				if (SG_MAP.IsExpOccupied())
				{
					Position newExpPos;
					if (SG_MAP.GetAddExpPt(newExpPos))
					{
						AddGoal(new BG_Expansion(newExpPos));
					}
					
				}
				else
					AddGoal(new BG_Expansion(SG_MAP.GetExpPt()));
			}
		}

		
	}

	Theme::Update(con);
}

std::string TH_ZerglingBase::ID()
{
	return "TH_ZerglingBase";
}
