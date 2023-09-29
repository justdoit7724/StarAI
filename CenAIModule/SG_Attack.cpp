#include "pch.h"
#include "SG_Attack.h"
#include "DebugManager.h"
#include "Controller.h"
#include "SituationManager.h"

SG_Attack::SG_Attack(GoalIO* passData)
	:SmallGoal(passData, "SG_Attack", Colors::Red)
{
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
			if (!u->exists())
				continue;

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

		m_isFinished = true;
		break;
	}
}

void SG_Attack::Debug()
{
	SG_DEBUGMGR.DrawTextScn(TilePosition(m_attPos), m_id);
	SG_DEBUGMGR.DrawBox(m_attPos, 90, 30, m_debugColor);
}

void SG_Attack::Init()
{
	m_attPos = m_passData->attPos;
}
