#include "pch.h"
#include "BG_TrainTroop.h"
#include "SituationManager.h"
#include "SG_Train.h"
#include "DebugManager.h"

BG_TrainTroop::BG_TrainTroop(Unit rd, std::vector<Unit> units, std::vector<float> weighs)
	:BigGoal(Colors::Red), m_rd(rd)
{
	/*if (!m_rd || !m_rd->exists())
		return;

	m_passData.dbBigGoalPos = m_rd->getPosition();
	m_passData.bigGoalPtr = this;

	
	if (SG_SITU.GetValidSupply() >= 3)
	{
		m_passData.unitTypes.push(UnitTypes::Zerg_Zergling);
		m_passData.iValues.push(6);
		m_passData.units.push(m_rd);

		m_subGoals.push_back(new SG_Train(&m_passData));
	}
	else
	{
		m_passData.unitTypes.push(UnitTypes::Zerg_Overlord);
		m_passData.iValues.push(1);
		m_passData.units.push(m_rd);

		m_subGoals.push_back(new SG_Train(&m_passData));
	}*/
}

void BG_TrainTroop::Debug(int depth)
{
	SG_DEBUGMGR.DrawTextScn(m_rd->getPosition(), ID());
	SG_DEBUGMGR.DrawBox(m_rd->getPosition(), 80, 25, m_debugColor);
}
