#include "pch.h"
#include "BG_Develop.h"
#include "TechTree.h"
#include "SituationManager.h"
#include "SG_Train.h"
#include "SG_StartWork.h"
#include "SG_Build.h"
#include "SG_Morphing.h"
#include "DebugManager.h"


BG_Develop::BG_Develop(Unit resourceDepot, std::vector<UnitType> units, std::vector<int> uCounts, float e):m_resourceDepot(resourceDepot), m_e(e), m_finish(false), BigGoal(Color(0,0,255))
{
	m_devKind = BG_DevelopeKind::None;

	if (!m_resourceDepot || !m_resourceDepot->exists())
		return;

	m_passData.dbBigGoalPos = m_resourceDepot->getPosition();
	m_passData.bigGoalPtr = this;

	int troopMinPrice = 0;
	int troopGasPrice = 0;
	for (int i = 0; i < units.size(); ++i)
	{
		int m, g;
		SG_SITU.GetUnitPrice(units[i],m, g);
		troopMinPrice += m * uCounts[i];
		troopGasPrice+=g * uCounts[i];
	}

	TechNode req;
	bool isInDev;
	if (SG_TECH.IsValid(UnitTypes::Zerg_Zergling, isInDev, &req) && 
		SG_SITU.CurMineral() >= troopMinPrice &&
		SG_SITU.CurGas()>=troopGasPrice)
	{
		if (SG_SITU.GetValidSupply() >= 3)
		{
			for (int i=0; i< units.size(); ++i)
			{
				m_passData.unitTypes.push(units[i]);
				m_passData.iValues.push(uCounts[i]);
			}
			m_passData.count = units.size();
			m_passData.units.push(m_resourceDepot);

			m_subGoals.push_back(new SG_Train(&m_passData));

			m_devKind = BG_DevelopeKind::Train_Troop;
		}
		else
		{
			m_passData.unitTypes.push(UnitTypes::Zerg_Overlord);
			m_passData.iValues.push(1);
			m_passData.units.push(m_resourceDepot);

			m_subGoals.push_back(new SG_Train(&m_passData));

			m_devKind = BG_DevelopeKind::Train_Supply;
		}

	}
	else
	{
		int reqMinPrice; 
		int reqGasPrice;
		SG_SITU.GetUnitPrice(req.unitType, reqMinPrice, reqGasPrice);

		Position pos;
		if (!isInDev && SG_SITU.CurMineral() >= reqMinPrice && SG_SITU.CurGas() >= reqGasPrice && SG_SITU.GetOpenPositionNear(m_resourceDepot->getPosition(), pos))
		{

			m_passData.unitTypes.push(req.unitType);
			m_passData.poses.push(pos);

			m_subGoals.push_back(new SG_Build(&m_passData));

			m_devKind = BG_DevelopeKind::Build_Tech;
		}
	}

	if (m_subGoals.size())
		return;

	
	if (SG_SITU.OpenMinerals(m_resourceDepot))
	{
		m_passData.unitTypes.push(UnitTypes::Zerg_Drone);
		m_passData.iValues.push(1);
		m_passData.units.push(m_resourceDepot);
		m_passData.bValues.push(true);

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
		//gas
		Unit gas = SG_SITU.GetGasNear(m_resourceDepot->getPosition());
		if (gas->getType() != UnitTypes::Zerg_Extractor)
		{
			m_passData.unitTypes.push(UnitTypes::Zerg_Extractor);
			m_passData.units.push(gas);
			m_subGoals.push_back(new SG_Build(&m_passData));
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
					m_passData.unitTypes.push(UnitTypes::Zerg_Sunken_Colony);
					m_passData.units.push(creepCount[0]);

					m_subGoals.push_back(new SG_Morphing(&m_passData));

					m_devKind = BG_DevelopeKind::Build_Defense;
				}
				else
				{
					Position pos;
					if (SG_SITU.GetOpenPositionNear(m_resourceDepot->getPosition(), pos))
					{
						m_passData.unitTypes.push(UnitTypes::Zerg_Creep_Colony);
						m_passData.unitTypes.push(UnitTypes::Zerg_Sunken_Colony);
						m_passData.poses.push(pos);

						m_subGoals.push_back(new SG_Build(&m_passData));
						m_subGoals.push_back(new SG_Morphing(&m_passData));

						m_devKind = BG_DevelopeKind::Build_Defense;
					}
				}

			}
		}
	}
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
	std::string str =  "BG_Develop:"+ std::to_string((int)kind);

	switch (kind)
	{
	case BG_DevelopeKind::Gathering:
	{
		str += std::to_string(rand());
	}
	break;
	}


	return str; 
}
