#include "pch.h"
#include "BG_Develop.h"
#include "TechTree.h"
#include "SituationManager.h"
#include "SG_Train.h"
#include "SG_StartWork.h"
#include "SG_Build.h"
#include "DebugManager.h"


BG_Develop::BG_Develop(Unit resourceDepot, float e):m_resourceDepot(resourceDepot), m_e(e), m_finish(false), BigGoal("BG_Develop",Color(0,0,255))
{
	m_passData.dbBigGoalPos = m_resourceDepot->getTilePosition();
	m_passData.bigGoalPtr = this;

	//train
	if (m_e > 0.5)
	{
		int techMineral;
		int techGas;
		SG_TECH.GetTotalReqRes(UnitTypes::Zerg_Zergling, techMineral, techGas);
		int curMinerals = Broodwar->self()->gatheredMinerals();

		TechNode req;
		if (SG_TECH.IsValid(UnitTypes::Zerg_Zergling, &req))
		{
			m_passData.unitTypes.clear();
			m_passData.unitTypes.push_back(UnitTypes::Zerg_Zergling);
			m_passData.iValues.clear();
			m_passData.iValues.push_back(6);
			m_passData.units.clear();
			m_passData.units.push_back(m_resourceDepot);

			m_subGoals.push_back(new SG_Train(&m_passData));
		}
		else
		{
			TilePosition tPos;
			if (SG_SITU.GetOpenPositionNear(SG_SITU.GetCenterTPos(m_resourceDepot), tPos))
			{

				m_passData.unitTypes.clear();
				m_passData.unitTypes.push_back(req.unitType);
				m_passData.poses.clear();
				m_passData.poses.push_back(tPos);

				m_subGoals.push_back(new SG_Build(&m_passData));
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
	}
	else
	{
		int sunkenCount = 0;
		sunkenCount = SG_SITU.UnitsInRange(true, m_resourceDepot->getTilePosition(), UnitTypes::Zerg_Sunken_Colony, 250).size();

		if (sunkenCount < 3)
		{
			TilePosition tPos;
			if (SG_SITU.GetOpenPositionNear(m_resourceDepot->getTilePosition(), tPos))
			{
				m_passData.unitTypes.clear();
				m_passData.unitTypes.push_back(UnitTypes::Zerg_Sunken_Colony);
				m_passData.poses.clear();
				m_passData.poses.push_back(tPos);

				m_subGoals.push_back(new SG_Build(&m_passData));
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
	std::string str = m_id + ":" + std::to_string(m_e);
	
	SG_DEBUGMGR.DrawTextScn(m_resourceDepot->getTilePosition(), str);
	SG_DEBUGMGR.DrawBox(m_resourceDepot->getTilePosition(), 100, 30, m_debugColor);
}
