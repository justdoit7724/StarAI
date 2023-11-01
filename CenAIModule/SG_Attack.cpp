#include "pch.h"
#include "SG_Attack.h"
#include "DebugManager.h"
#include "Controller.h"
#include "SituationManager.h"
#include "MapManager.h"
#include "LogManager.h"

SG_Attack::SG_Attack(GoalIO* passData)
	:SmallGoal(passData, Colors::Red)
{
}

SG_Attack::~SG_Attack()
{
	auto units = SG_SITU.GetRegistered(m_passData->bigGoalPtr);

	for (auto u : units)
	{
		SG_SITU.UnregisterUnit(u);
	}

}

void SG_Attack::Update(const Controller* con)
{
	if (Finished())
		return;

	switch (m_stage)
	{
	case 1:
	{
		auto units = SG_SITU.GetRegistered(m_passData->bigGoalPtr);

		for (auto u : units)
		{
			con->Attack(u, m_attPos);
		}
		m_stage++;
	}

		break;

	case 2:
	{
		auto units = SG_SITU.GetRegistered(m_passData->bigGoalPtr);

		bool isAllIdle = true;
		for (auto u : units)
		{
			if (!u->isIdle())
			{
				isAllIdle = false;
				break;
			}
		}

		if (isAllIdle)
			m_stage++;
	}
		break;

	default:
			m_result = GOAL_RESULT_SUCCESS;
		break;
	}
}

void SG_Attack::Debug(int depth)
{
	SG_DEBUGMGR.DrawTextScn(m_attPos, "Attack");
	SG_DEBUGMGR.DrawBox(m_attPos, 90, 30, m_debugColor);
}

void SG_Attack::Init()
{
	if (!SG_MAP.IsValidPos(m_passData->attPos))
	{

		SG_LOGMGR.Record("GOAL_EXCEPT", "unvalid attPos in Attack");
		m_result = GOAL_RESULT_FAILED;
		return;
	}

	m_attPos = m_passData->attPos;
}
