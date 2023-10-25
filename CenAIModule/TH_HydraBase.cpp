#include "pch.h"
#include "TH_HydraBase.h"
#include "SituationManager.h"
#include "TechTree.h"
#include "MapManager.h"
#include "BG_Attack.h"
#include "BG_Develop.h"

TH_HydraBase::TH_HydraBase()
	:Theme(ID())
{
}

void TH_HydraBase::Update(const Controller* con)
{
	auto rd = SG_SITU.UnitsInRange(true, TilePosition(32, 32), UnitTypes::Zerg_Hatchery);

	if (rd.size())
	{
		auto hydras = SG_SITU.UnitsInRange(true, rd[0]->getTilePosition(), UnitTypes::Zerg_Hydralisk, 10000);
		int Zerg_Hydralisk = 0;
		for (int i = 0; i < hydras.size(); ++i)
		{
			if (!SG_SITU.IsUnitRegistered(hydras[i]))
				Zerg_Hydralisk++;
		}

		if (Zerg_Hydralisk >= 6)
		{
			auto pts = SG_MAP.GetPts();

			AddGoal(new BG_Attack(Position(pts[0])));
		}
		else
		{
			auto newGoal = new BG_Develop(rd[0], { UnitTypes::Zerg_Hydralisk }, { 3 }, 0);
			if (!IsGoalExist(newGoal))
			{
				AddGoal(newGoal);
			}
			else
				delete newGoal;
		}
	}

	Theme::Update(con);
}

std::string TH_HydraBase::ID()
{
	return "TH_HydraBase";
}
