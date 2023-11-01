#include "pch.h"
#include "TH_FastAttBase.h"
#include "StopWatch.h"
#include "SituationManager.h"
#include "MapManager.h"
#include "BG_Attack.h"
#include "BG_Develop.h"

TH_FastAttBase::TH_FastAttBase()
    :Theme(GetID())
{
	m_timer = new StopWatch();
	m_totalTroopCount = 6;
}

TH_FastAttBase::~TH_FastAttBase()
{
	delete m_timer;
}

void TH_FastAttBase::Update(const Controller* con)
{
	/*auto mPL = SG_SITU.GetPL() + m_plCompensate;
	if (mPL < -150)
	{
		m_hydraCount += 2;
		m_plCompensate += 150;
	}
	else if (mPL > 150)
	{
		m_hydraCount -= 2;
		m_plCompensate -= 150;
		if (m_hydraCount < 12)
			m_hydraCount = 12;
	}*/

	if (m_totalTroopCount == 0)
	{
		if (m_timer->elapsed() > 1)
		{
			m_isFinished = true;
			return;
		}
	}

	if (m_timer->elapsed() > 0.2 && m_totalTroopCount > 0)
	{
		m_timer->reset();

		std::vector<UnitType> units;
		std::vector<int> unitCounts;
		std::vector<UnitType> trainUnits;
		std::vector<int> trainUnitCounts;
		units.push_back(UnitTypes::Zerg_Zergling);
		unitCounts.push_back(6);


		bool isTroopReady = true;
		for (int i = 0; i < units.size(); ++i)
		{
			auto curUnits = SG_SITU.UnitsInRange(true, TilePosition(0, 0), units[i], 10000);
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

			m_totalTroopCount -= unitCounts[0];

			AddGoal(new BG_Attack(Position(pts[0]), units, unitCounts));
		}

		auto rd = SG_SITU.UnitsInRange(true, TilePosition(32, 32), UnitTypes::Zerg_Hatchery);
		
		if(rd.size() && m_totalTroopCount > 0)
		{
			
			AddGoal(new BG_Develop(rd[0], trainUnits, trainUnitCounts, true));
		}
	}

	Theme::Update(con);
}

std::string TH_FastAttBase::GetID()
{
    return "TH_FastAttBase";
}
