#include "pch.h"
#include "TH_HydraBase.h"
#include "SituationManager.h"
#include "TechTree.h"
#include "MapManager.h"
#include "BG_Attack.h"
#include "BG_Develop.h"
#include "StopWatch.h"

TH_HydraBase::TH_HydraBase()
	:Theme(ID()), m_plCompensate(0), m_hydraCount(12)
{
	m_timer = new StopWatch();
	m_timer2 = new StopWatch();
}

TH_HydraBase::~TH_HydraBase()
{
	delete m_timer;
	delete m_timer2;
}

void TH_HydraBase::Update(const Controller* con)
{
	if (m_timer2->elapsed() > 5)
	{
		m_timer2->reset();

		auto mPL = SG_SITU.GetPL() + m_plCompensate;
		if (mPL < -150)
		{
			m_hydraCount += 2;
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
			units.push_back(UnitTypes::Zerg_Hydralisk);
			unitCounts.push_back(m_hydraCount);

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
					trainUnitCounts.push_back(unitCounts[i] - curUnitCount);
				}
			}

			if (isTroopReady)
			{
				auto pts = SG_MAP.GetPts();

				AddGoal(new BG_Attack(Position(pts[0]), units, unitCounts));
			}

			AddGoal(new BG_Develop(rd[0], trainUnits, trainUnitCounts, 0));
		}
	}

	Theme::Update(con);
}

std::string TH_HydraBase::ID()
{
	return "TH_HydraBase";
}
