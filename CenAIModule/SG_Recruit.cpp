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


	m_result = GOAL_RESULT_SUCCESS;

	std::vector<std::vector<Unit>> validUnits(m_units.size());
	for (int i = 0; i < m_units.size(); ++i)
	{
		auto curUnits = SG_SITU.UnitsInRange(true, TilePosition(0, 0), m_units[i], 10000);
		for (auto u : curUnits)
		{
			if (SG_SITU.IsUnitRegistered(u))
				continue;

			validUnits[i].push_back(u);

			if (m_unitCounts[i] == validUnits[i].size())
				break;
		}

		if (m_unitCounts[i] > validUnits[i].size())
		{
			SG_LOGMGR.Record("GOAL_EXCEPT", "not enough zerglings");
			m_result = GOAL_RESULT_FAILED;
			break;
			
		}
	}

	if (m_result == GOAL_RESULT_SUCCESS)
	{

		for (auto units : validUnits)
			for (auto u : units)
			{
				SG_SITU.RegisterUnit(m_passData->bigGoalPtr, u);
			}
	}
}

void SG_Recruit::Debug(int depth)
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

	if (m_passData->iValues[this].empty() || m_passData->unitTypes[this].empty())
	{
		SG_LOGMGR.Record("GOAL_EXCEPT", "no unit number assigned");
		m_result = GOAL_RESULT_FAILED;
		return;
	}

	for (int i = 0; i < m_passData->iValues[this].size(); ++i)
	{
		m_units.push_back(m_passData->unitTypes[this][i]);
		m_unitCounts.push_back(m_passData->iValues[this][i]);
	}

	
}
