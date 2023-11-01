#include "pch.h"
#include "TH_HydraBase.h"
#include "SituationManager.h"
#include "TechTree.h"
#include "MapManager.h"
#include "BG_Attack.h"
#include "BG_Develop.h"
#include "StopWatch.h"
#include "BG_Expansion.h"

TH_HydraBase::TH_HydraBase()
	:Theme(GetID()), m_plCompensate(0), m_hydraCount(12), m_curRD(0)
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
	auto mPL = SG_SITU.GetPL() + m_plCompensate;
	if (mPL < -150)
	{
		m_hydraCount += 2;
		m_plCompensate += 150;
	}
	else if(mPL>150)
	{
		m_hydraCount -= 2;
		m_plCompensate -= 150;
		if (m_hydraCount < 12)
			m_hydraCount = 12;
	}

	if (m_timer->elapsed() > 0.25)
	{
		m_timer->reset();


		std::vector<UnitType> units;
		std::vector<int> unitCounts;
		std::vector<UnitType> trainUnits;
		std::vector<int> trainUnitCounts;
		units.push_back(UnitTypes::Zerg_Hydralisk);
		unitCounts.push_back(m_hydraCount);

		bool isTroopReady = true;
		for (int i = 0; i < units.size(); ++i)
		{
			auto curUnits = SG_SITU.UnitsInRange(true, TilePosition(0,0), units[i], 10000);
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

		auto rd = SG_SITU.UnitsInRange(true, TilePosition(32, 32), UnitTypes::Zerg_Hatchery);
		if (m_curRD >= rd.size())
			m_curRD = 0;

		while (m_curRD < rd.size())
		{
			if (rd[m_curRD]->isMorphing())
			{
				m_curRD++;
				continue;
			}

			AddGoal(new BG_Develop(rd[m_curRD], trainUnits, trainUnitCounts, false));

			if (((SG_SITU.CurMineral() > 500) || (SG_SITU.GetCurMDifAvg() > 15 && SG_SITU.CurMineral() > 250)) && !SG_SITU.IsDeveloping(UnitTypes::Zerg_Hatchery))
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

			break;
		}

		m_curRD++;
	}

	Theme::Update(con);
}

std::string TH_HydraBase::GetID()
{
	return "TH_HydraBase";
}
