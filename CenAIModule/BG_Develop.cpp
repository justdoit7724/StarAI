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

	//supply
	//mineral
	//gas
	//tech
	// 
	//defense
	
	auto drones = SG_SITU.UnitsInRange(true, m_resourceDepot, UnitTypes::Zerg_Drone);
	
	if (SG_SITU.GetValidSupply() < 1 && SG_SITU.IsDeveloping(UnitTypes::Zerg_Overlord)==0)
	{
		auto supplyGoal = new SG_Train(&m_passData);

		m_passData.unitTypes[supplyGoal].push_back(UnitTypes::Zerg_Overlord);
		m_passData.iValues[supplyGoal].push_back(1);
		m_passData.units[supplyGoal].push_back(m_resourceDepot);
		m_passData.devs[supplyGoal].push_back({ UnitTypes::Zerg_Overlord,1 });

		m_subGoals.push_back(supplyGoal);

		m_devKind = BG_DevelopeKind::Train_Supply;
	}
	

	int om = SG_SITU.OpenMinerals(m_resourceDepot);
	int dd = SG_SITU.IsDeveloping(UnitTypes::Zerg_Drone);
	if (om > dd)
	{
		bool isFreeWorker = false;
		for (auto d : drones)
		{
			if (!d->isIdle())
				continue;
			if (SG_SITU.IsUnitRegistered(d))
				continue;

			isFreeWorker = true;
			break;
		}

		auto startWorkGoal = new SG_StartWork(&m_passData);
		if (!isFreeWorker)
		{
			auto trainDroneGoal = new SG_Train(&m_passData);
			m_passData.unitTypes[trainDroneGoal].push_back(UnitTypes::Zerg_Drone);
			m_passData.iValues[trainDroneGoal].push_back(1);
			m_passData.units[trainDroneGoal].push_back(m_resourceDepot);
			m_passData.devs[trainDroneGoal].push_back({ UnitTypes::Zerg_Drone,1 });

			m_subGoals.push_back(trainDroneGoal);
		}

		m_passData.units[startWorkGoal].push_back(m_resourceDepot);
		m_passData.bValues[startWorkGoal].push_back(true);
		m_subGoals.push_back(startWorkGoal);


		m_devKind = BG_DevelopeKind::Gathering;
	}
	else
	{
		int gasWorkerCount = 0;
		for (auto w : drones)
		{
			if (w->isGatheringGas())
				gasWorkerCount++;
		}
		if (gasWorkerCount < 3 && !SG_SITU.IsDeveloping(UnitTypes::Zerg_Extractor))
		{
			Unit gas = SG_SITU.GetGasNear(m_resourceDepot->getPosition());
			if (gas->getType() != UnitTypes::Zerg_Extractor)
			{
				auto buildGasGoal = new SG_Build(&m_passData);

				m_passData.unitTypes[buildGasGoal].push_back(UnitTypes::Zerg_Extractor);
				m_passData.poses[buildGasGoal].push_back(gas->getPosition());
				m_passData.devs[buildGasGoal].push_back({ UnitTypes::Zerg_Extractor,1 });

				m_subGoals.push_back(buildGasGoal);

				m_devKind = BG_DevelopeKind::Build_Gas;

			}
			else
			{
				bool isFreeWorker = false;
				for (auto d : drones)
				{
					if (!d->isIdle())
						continue;
					if (SG_SITU.IsUnitRegistered(d))
						continue;

					isFreeWorker = true;
					break;
				}

				if (!isFreeWorker)
				{
					auto trainDroneGoal = new SG_Train(&m_passData);

					m_passData.unitTypes[trainDroneGoal].push_back(UnitTypes::Zerg_Drone);
					m_passData.iValues[trainDroneGoal].push_back(1);
					m_passData.units[trainDroneGoal].push_back(m_resourceDepot);
					m_passData.devs[trainDroneGoal].push_back({ UnitTypes::Zerg_Drone,1 });
					m_subGoals.push_back(trainDroneGoal);
				}

				auto startWorkGoal = new SG_StartWork(&m_passData);
				m_passData.units[startWorkGoal].push_back(m_resourceDepot);
				m_passData.bValues[startWorkGoal].push_back(false);
				m_subGoals.push_back(startWorkGoal);

				m_devKind = BG_DevelopeKind::Gathering;

			}
		}
	}

	int troopMinPrice = 0;
	int troopGasPrice = 0;
	for (int i = 0; i < units.size(); ++i)
	{
		int m, g;
		SG_SITU.GetUnitPrice(units[i], m, g);
		troopMinPrice += m * uCounts[i];
		troopGasPrice += g * uCounts[i];

		if (units[i] == UnitTypes::Zerg_Zergling)
			troopMinPrice /= 2;
	}

	TechNode req;
	bool isInDev;
	bool isTechValid = SG_TECH.IsValid(UnitTypes::Zerg_Zergling, isInDev, &req);
	if (isTechValid)
	{
		auto trainGoal = new SG_Train(&m_passData);
		
		for (int i = 0; i < units.size(); ++i)
		{
			m_passData.unitTypes[trainGoal].push_back(units[i]);
			m_passData.iValues[trainGoal].push_back(uCounts[i]);

			m_passData.devs[trainGoal].push_back({ units[i],uCounts[i] });
		}
		m_passData.count = units.size();
		m_passData.units[trainGoal].push_back(m_resourceDepot);

		m_subGoals.push_back(trainGoal);

	}
	else
	{
		int reqMinPrice;
		int reqGasPrice;
		SG_SITU.GetUnitPrice(req.unitType, reqMinPrice, reqGasPrice);

		Position pos;
		if (SG_SITU.CurMineral() >= reqMinPrice && SG_SITU.CurGas() >= reqGasPrice && SG_SITU.GetOpenPositionNear(m_resourceDepot->getPosition(), pos))
		{
			if (!SG_SITU.IsDeveloping(req.unitType))
			{

				auto buildGoal = new SG_Build(&m_passData);

				m_passData.unitTypes[buildGoal].push_back(req.unitType);
				m_passData.poses[buildGoal].push_back(pos);
				m_passData.devs[buildGoal].push_back({ req.unitType,1 });

				m_subGoals.push_back(buildGoal);

				m_devKind = BG_DevelopeKind::Build_Tech;
			}
		}


	}

	if (isTechValid)
	{
		int sunkenCount = 0;
		sunkenCount = SG_SITU.UnitsInRange(true, m_resourceDepot->getTilePosition(), UnitTypes::Zerg_Sunken_Colony, 250).size();

		if (sunkenCount < 2)
		{
			auto creepCount = SG_SITU.UnitsInRange(true, m_resourceDepot->getTilePosition(), UnitTypes::Zerg_Creep_Colony, 250);
			/*if (creepCount.size())
			{
				auto morphingGoal = new SG_Morphing(&m_passData);

				m_passData.unitTypes[morphingGoal].push_back(UnitTypes::Zerg_Sunken_Colony);
				m_passData.units[morphingGoal].push_back(creepCount[0]);
				m_passData.devs[morphingGoal].push_back({ UnitTypes::Zerg_Sunken_Colony,1 });

				m_subGoals.push_back(morphingGoal);

				m_devKind = BG_DevelopeKind::Build_Defense;
			}*/
			if (!SG_SITU.IsDeveloping(UnitTypes::Zerg_Sunken_Colony))
			{
				Position pos;
				if (SG_SITU.GetOpenPositionNear(m_resourceDepot->getPosition(), pos))
				{
					auto buildGoal = new SG_Build(&m_passData);
					auto morphGoal = new SG_Morphing(&m_passData);

					m_passData.unitTypes[buildGoal].push_back(UnitTypes::Zerg_Creep_Colony);
					m_passData.unitTypes[morphGoal].push_back(UnitTypes::Zerg_Sunken_Colony);
					m_passData.poses[buildGoal].push_back(pos);
					m_passData.devs[buildGoal].push_back({ UnitTypes::Zerg_Sunken_Colony,1 });
					m_passData.devs[morphGoal].push_back({ UnitTypes::Zerg_Sunken_Colony,1 });

					m_subGoals.push_back(buildGoal);
					m_subGoals.push_back(morphGoal);

					m_devKind = BG_DevelopeKind::Build_Defense;
				}
			}

		}
	}


	
	
	/*if (SG_SITU.GetCurMDifAvg() < 17)*/
	

	
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
