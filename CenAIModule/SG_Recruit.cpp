#include "pch.h"
#include "SG_Recruit.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "LogManager.h"

SG_Recruit::SG_Recruit(GoalIO* passData)
	:SmallGoal(passData, Color(0, 0, 155))
{
}
void SG_Recruit::Update(const Controller* con)
{
	if (Finished())
		return;

	auto zerglings = SG_SITU.UnitsInRange(true, TilePosition(0, 0), UnitTypes::Zerg_Zergling, 10000);
	std::vector<Unit> validZerglings;
	for (auto z : zerglings)
	{
		if (SG_SITU.IsUnitRegistered(z))
			continue;

		validZerglings.push_back(z);
	}
	
	if (m_num <= validZerglings.size())
	{
		for (auto z : validZerglings)
		{
			SG_SITU.RegisterUnit(m_passData->bigGoalPtr, z);
		}

		m_result = GOAL_RESULT_SUCCESS;
	}
	else
	{
		SG_LOGMGR.Record("GOAL_EXCEPT", "not enough zerglings");
		m_result = GOAL_RESULT_FAILED;
	}
}

void SG_Recruit::Debug()
{
	auto pos = m_passData->dbBigGoalPos;
	pos.y += 32;
	SG_DEBUGMGR.DrawTextScn(pos, "Recruit");
	SG_DEBUGMGR.DrawBox(pos, 100, 30, m_debugColor);
}

void SG_Recruit::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

	if (m_passData->iValues.empty() || m_passData->iValues[0] == 0)
	{
		SG_LOGMGR.Record("GOAL_EXCEPT", "no unit number assigned");
		m_result = GOAL_RESULT_FAILED;
		return;
	}

	m_num = m_passData->iValues[0];

	
}
