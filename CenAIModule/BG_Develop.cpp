#include "pch.h"
#include "BG_Develop.h"
#include "TechTree.h"
#include "SituationManager.h"
#include "SG_Train.h"
#include "SG_StartWork.h"
#include "SG_Build.h"
#include "SG_Morphing.h"
#include "DebugManager.h"


BG_Develop::BG_Develop(Unit resourceDepot, float e):m_resourceDepot(resourceDepot), m_e(e), m_finish(false), BigGoal(Color(0,0,255))
{
	m_devKind = BG_DevelopeKind::None;

	if (!m_resourceDepot || !m_resourceDepot->exists())
		return;

	m_passData.dbBigGoalPos = m_resourceDepot->getPosition();
	m_passData.bigGoalPtr = this;

	//train
	if (m_e > 0.5)
	{
		TechNode req;
		bool isInDev;
		if (SG_TECH.IsValid(UnitTypes::Zerg_Zergling, isInDev, &req))
		{
			if (SG_SITU.GetValidSupply() >= 3)
			{
				m_passData.unitTypes.clear();
				m_passData.unitTypes.push_back(UnitTypes::Zerg_Zergling);
				m_passData.iValues.clear();
				m_passData.iValues.push_back(6);
				m_passData.units.clear();
				m_passData.units.push_back(m_resourceDepot);

				m_subGoals.push_back(new SG_Train(&m_passData));

				m_devKind = BG_DevelopeKind::Train_Troop;
			}
			else
			{
				m_passData.unitTypes.clear();
				m_passData.unitTypes.push_back(UnitTypes::Zerg_Overlord);
				m_passData.iValues.clear();
				m_passData.iValues.push_back(1);
				m_passData.units.clear();
				m_passData.units.push_back(m_resourceDepot);

				m_subGoals.push_back(new SG_Train(&m_passData));

				m_devKind = BG_DevelopeKind::Train_Supply;
			}
		}
		else
		{
			Position pos;
			if (!isInDev && SG_SITU.GetOpenPositionNear(m_resourceDepot->getPosition(), pos))
			{

				m_passData.unitTypes.clear();
				m_passData.unitTypes.push_back(req.unitType);
				m_passData.poses.clear();
				m_passData.poses.push_back(pos);

				m_subGoals.push_back(new SG_Build(&m_passData));

				m_devKind = BG_DevelopeKind::Build_Tech;
			}
		}

		return;
	}
	//develop
	
	if (SG_SITU.OpenMinerals(m_resourceDepot))
	{
		m_passData.unitTypes.clear();
		m_passData.unitTypes.push_back(UnitTypes::Zerg_Drone);
		m_passData.iValues.clear();
		m_passData.iValues.push_back(1);
		m_passData.units.clear();
		m_passData.units.push_back(m_resourceDepot);
		m_passData.bValues.clear();
		m_passData.bValues.push_back(true);

		bool isFreeWorker = false;
		auto drones = SG_SITU.UnitsInRange(true, m_resourceDepot, UnitTypes::Zerg_Drone);
		for (auto d : drones)
		{
			if (!d->isIdle())
				continue;
			if (SG_SITU.IsUnitRegistered(d))
				continue;

			isFreeWorker = true;
			break;
		}

		if(!isFreeWorker)
			m_subGoals.push_back(new SG_Train(&m_passData));
		m_subGoals.push_back(new SG_StartWork(&m_passData));


		m_devKind = BG_DevelopeKind::Gathering;
	}
	else
	{
		int sunkenCount = 0;
		sunkenCount = SG_SITU.UnitsInRange(true, m_resourceDepot->getTilePosition(), UnitTypes::Zerg_Sunken_Colony, 250).size();

		if (sunkenCount < 3)
		{
			auto creepCount = SG_SITU.UnitsInRange(true, m_resourceDepot->getTilePosition(), UnitTypes::Zerg_Creep_Colony, 250);
			if (creepCount.size())
			{
				m_passData.unitTypes.clear();
				m_passData.unitTypes.push_back(UnitTypes::Zerg_Sunken_Colony);
				m_passData.units.clear();
				m_passData.units.push_back(creepCount[0]);

				m_subGoals.push_back(new SG_Morphing(&m_passData));

				m_devKind = BG_DevelopeKind::Build_Defense;
			}
			else
			{
				Position pos;
				if (SG_SITU.GetOpenPositionNear(m_resourceDepot->getPosition(), pos))
				{
					m_passData.unitTypes.clear();
					m_passData.unitTypes.push_back(UnitTypes::Zerg_Creep_Colony);
					m_passData.unitTypes.push_back(UnitTypes::Zerg_Sunken_Colony);
					m_passData.poses.clear();
					m_passData.poses.push_back(pos);
					m_passData.curIndex = 0;

					m_subGoals.push_back(new SG_Build(&m_passData));
					m_subGoals.push_back(new SG_Morphing(&m_passData));

					m_devKind = BG_DevelopeKind::Build_Defense;
				}
			}

		}
	}
	
}


void BG_Develop::Update(const Controller* con)
{
	if (Finished())
		return;


	BigGoal::Update(con);
}

void BG_Develop::Debug()
{
	SG_DEBUGMGR.DrawTextScn(m_resourceDepot->getPosition(), ID());
	SG_DEBUGMGR.DrawBox(m_resourceDepot->getPosition(), 100, 30, m_debugColor);
}

std::string BG_Develop::ID()const
{
	return ID(m_devKind);
}

std::string BG_Develop::ID(BG_DevelopeKind kind)
{
	return "BG_Develop:" + std::to_string((int)kind);

}
