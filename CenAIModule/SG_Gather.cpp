#include "pch.h"
#include "SG_Gather.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "Controller.h"
#include "LogManager.h"
#include "MapManager.h"

SG_Gather::SG_Gather(GoalIO* passData)
	:SmallGoal(passData, Color(100,255,100))
{
}

void SG_Gather::Update(const Controller* con)
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
			con->Attack(u, m_pos);
		}

		m_stage++;
	}
		break;
	case 2:
	{

		auto units = SG_SITU.GetRegistered(m_passData->bigGoalPtr);
		bool isAllGathered = true;
		for (auto u : units)
		{
			if (u->getDistance(Position(m_pos)) > 200)
			{
				isAllGathered = false;
				break;
			}
		}

		if (isAllGathered)
			m_stage++;
	}
		break;
	default:
		m_result=GOAL_RESULT_SUCCESS;
		break;
	}
}

void SG_Gather::Debug(int depth)
{
	SG_DEBUGMGR.DrawTextScn(m_pos, "Gather");
	SG_DEBUGMGR.DrawBox(m_pos, 90, 30, m_debugColor);
}

void SG_Gather::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

	TilePosition tPos;
	tPos.x = m_passData->attPos.x / 32;
	tPos.y = m_passData->attPos.y / 32;


	if (!SG_MAP.IsValidPos(m_passData->attPos))
	{
		SG_LOGMGR.Record("GOAL_EXCEPT", "unvalid attPos in Gather");
		m_result = GOAL_RESULT_FAILED;
		return;
	}
	m_pos = m_passData->attPos;
}
